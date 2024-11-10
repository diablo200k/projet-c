#include "repl.h"
#include "input.h"
#include "statement.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>  

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        free_table(table);
        exit(EXIT_SUCCESS);
    } else if (strcmp(input_buffer->buffer, ".help") == 0) {
        printf("Instructions d'utilisation :\n");
        printf("  insert <id> <username> <email>  - Insère une nouvelle ligne\n");
        printf("  select                          - Affiche toutes les lignes\n");
        printf("  select where id = <id>          - Affiche une ligne spécifique\n");
        printf("  update <id> set username=<username> email=<email> - Met à jour une ligne\n");
        printf("  delete where id = <id>          - Supprime une ligne\n");
        printf("  .exit                           - Quitte le programme\n");
        printf("  .help                           - Affiche ces instructions\n");
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

void repl(Table* table) {
    InputBuffer* input_buffer = new_input_buffer();
    printf("Bienvenue dans la base de données simple. Tapez '.help' pour les instructions.\n");
    while (true) {
        print_prompt();
        read_input(input_buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case META_COMMAND_SUCCESS:
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement.\n");
                continue;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at start of '%s'.\n", input_buffer->buffer);
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
        }

        switch (execute_statement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed.\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table full.\n");
                break;
            case EXECUTE_DUPLICATE_KEY:
                printf("Error: Duplicate key.\n");
                break;
            case EXECUTE_FAILURE:
                printf("Error: Execution failed.\n");
                break;
        }
    }
}