#include "statement.h"
#include "input.h"
#include "table.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        char username[COLUMN_USERNAME_SIZE];
        char email[COLUMN_EMAIL_SIZE];
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", 
                                   &(statement->row_to_insert.id), 
                                   username,
                                   email);
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        if (strlen(username) > COLUMN_USERNAME_SIZE) {
            return PREPARE_STRING_TOO_LONG;
        }
        if (strlen(email) > COLUMN_EMAIL_SIZE) {
            return PREPARE_STRING_TOO_LONG;
        }
        strcpy(statement->row_to_insert.username, username);
        strcpy(statement->row_to_insert.email, email);
        return PREPARE_SUCCESS;
    }
    
    if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        if (strcmp(input_buffer->buffer, "select") == 0) {
            statement->type = STATEMENT_SELECT;
            statement->condition_column = CONDITION_NONE;
            return PREPARE_SUCCESS;
        } else {
            int matches = sscanf(input_buffer->buffer, "select where id = %d", &statement->condition_value);
            if (matches == 1) {
                statement->type = STATEMENT_SELECT;
                statement->condition_column = CONDITION_ID;
                return PREPARE_SUCCESS;
            }
        }
        return PREPARE_SYNTAX_ERROR;
    }
    
    return PREPARE_UNRECOGNIZED_STATEMENT;
}


ExecuteResult execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        case STATEMENT_SELECT:
            return execute_select(statement, table);
        default:
            return EXECUTE_FAILURE;
    }
}

ExecuteResult execute_insert(Statement* statement, Table* table) {
    Row* row_to_insert = &(statement->row_to_insert);

    // Vérifier si l'ID existe déjà
    FILE* file = fopen("database_backup.txt", "r");
    if (file != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            int id;
            if (sscanf(line, "%d,", &id) == 1 && id == row_to_insert->id) {
                fclose(file);
                printf("Erreur : ID %d existe déjà.\n", id);
                return EXECUTE_DUPLICATE_KEY;
            }
        }
        fclose(file);
    }

    // Ajouter la nouvelle ligne au fichier
    file = fopen("database_backup.txt", "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return EXECUTE_FAILURE;
    }
    fprintf(file, "%d,%s,%s\n", row_to_insert->id, row_to_insert->username, row_to_insert->email);
    fclose(file);

    printf("Inserted 1 row.\n");
    return EXECUTE_SUCCESS;
}

#define MAX_COLUMN_WIDTH 20

void print_separator(int id_width, int username_width, int email_width) {
    printf("+-%*s-+-%*s-+-%*s-+\n", id_width, "-", username_width, "-", email_width, "-");
}

void print_row_formatted(int id, const char* username, const char* email, int id_width, int username_width, int email_width) {
    printf("| %*d | %-*.*s | %-*.*s |\n", 
           id_width, id, 
           username_width, username_width, username, 
           email_width, email_width, email);
}

ExecuteResult execute_select(Statement* statement, Table* table) {
    FILE* file = fopen("database_backup.txt", "r");
    if (file == NULL) {
        printf("Aucune donnée à afficher.\n");
        return EXECUTE_SUCCESS;
    }

    int id_width = 5;
    int username_width = 10;
    int email_width = 20;

    // Première passe pour déterminer les largeurs des colonnes
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        int id;
        char username[COLUMN_USERNAME_SIZE];
        char email[COLUMN_EMAIL_SIZE];
        if (sscanf(line, "%d,%[^,],%s", &id, username, email) == 3) {
            if (statement->condition_column == CONDITION_NONE || 
                (statement->condition_column == CONDITION_ID && id == statement->condition_value)) {
                int id_len = snprintf(NULL, 0, "%d", id);
                int username_len = strlen(username);
                int email_len = strlen(email);

                if (id_len > id_width) id_width = id_len;
                if (username_len > username_width) username_width = username_len;
                if (email_len > email_width) email_width = email_len;
            }
        }
    }

    // Ajouter un peu d'espace supplémentaire pour l'esthétique
    id_width += 2;
    username_width += 2;
    email_width += 2;

    // Afficher l'en-tête
    print_separator(id_width, username_width, email_width);
    printf("| %-*s | %-*s | %-*s |\n", id_width, "ID", username_width, "Username", email_width, "Email");
    print_separator(id_width, username_width, email_width);

    // Réinitialiser le fichier pour la deuxième passe
    rewind(file);

    // Afficher les données
    int rows_displayed = 0;
    while (fgets(line, sizeof(line), file)) {
        int id;
        char username[COLUMN_USERNAME_SIZE];
        char email[COLUMN_EMAIL_SIZE];
        if (sscanf(line, "%d,%[^,],%s", &id, username, email) == 3) {
            if (statement->condition_column == CONDITION_NONE || 
                (statement->condition_column == CONDITION_ID && id == statement->condition_value)) {
                print_row_formatted(id, username, email, id_width, username_width, email_width);
                rows_displayed++;
            }
        }
    }

    print_separator(id_width, username_width, email_width);

    fclose(file);

    printf("%d rows displayed.\n", rows_displayed);
    return EXECUTE_SUCCESS;
}

void serialize_row(Row* source, void* destination) {
    memcpy(destination, &(source->id), sizeof(uint32_t));
    strncpy((char*)destination + sizeof(uint32_t), source->username, COLUMN_USERNAME_SIZE);
    strncpy((char*)destination + sizeof(uint32_t) + COLUMN_USERNAME_SIZE, source->email, COLUMN_EMAIL_SIZE);
}

void deserialize_row(void* source, Row* destination) {
    memcpy(&(destination->id), source, sizeof(uint32_t));
    strncpy(destination->username, (char*)source + sizeof(uint32_t), COLUMN_USERNAME_SIZE);
    strncpy(destination->email, (char*)source + sizeof(uint32_t) + COLUMN_USERNAME_SIZE, COLUMN_EMAIL_SIZE);
}

void print_row(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}

void write_to_file(Row* row) {
    FILE* file = fopen("database_backup.txt", "a");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier de sauvegarde.\n");
        return;
    }
    fprintf(file, "%d,%s,%s\n", row->id, row->username, row->email);
    fclose(file);
}