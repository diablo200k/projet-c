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
    if (strcmp(input_buffer->buffer, "select") == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult execute_statement(Statement* statement, Table* table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return execute_insert(statement, table);
        case STATEMENT_SELECT:
            return execute_select(table);
        default:
            return EXECUTE_FAILURE;
    }
}

ExecuteResult execute_insert(Statement* statement, Table* table) {
    if (table->num_rows >= ROWS_PER_PAGE * TABLE_MAX_PAGES) {
        return EXECUTE_TABLE_FULL;
    }

    Row* row_to_insert = &(statement->row_to_insert);
    void* page = row_slot(table, table->num_rows);
    if (page == NULL) {
        return EXECUTE_FAILURE;
    }
    serialize_row(row_to_insert, page);
    table->num_rows += 1;

    return EXECUTE_SUCCESS;
}

ExecuteResult execute_select(Table* table) {
    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        void* page = row_slot(table, i);
        if (page == NULL) {
            return EXECUTE_FAILURE;
        }
        deserialize_row(page, &row);
        print_row(&row);
    }
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