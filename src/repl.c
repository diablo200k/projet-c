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
    }
    return META_COMMAND_UNRECOGNIZED_COMMAND;
}

void repl(Table* table) {
    InputBuffer* input_buffer = new_input_buffer();
    while (true) {
        printf("Debug: Starting REPL iteration\n");
        print_prompt();
        read_input(input_buffer);
        printf("Debug: Input read: %s\n", input_buffer->buffer);

        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer, table)) {
                case META_COMMAND_SUCCESS:
                    printf("Debug: Meta command executed successfully\n");
                    continue;
                case META_COMMAND_UNRECOGNIZED_COMMAND:
                    printf("Unrecognized command '%s'\n", input_buffer->buffer);
                    continue;
            }
        }

        Statement statement;
        PrepareResult prepare_result = prepare_statement(input_buffer, &statement);
        printf("Debug: Prepare result: %d\n", prepare_result);

        switch (prepare_result) {
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

        ExecuteResult execute_result = execute_statement(&statement, table);
        printf("Debug: Execute result: %d\n", execute_result);

        switch (execute_result) {
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