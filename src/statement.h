#ifndef STATEMENT_H
#define STATEMENT_H

#include "input.h"
#include "table.h"

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_SYNTAX_ERROR,
    PREPARE_STRING_TOO_LONG
} PrepareResult;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_DUPLICATE_KEY,
    EXECUTE_FAILURE
} ExecuteResult;



typedef enum { CONDITION_NONE, CONDITION_ID } ConditionColumn;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
    STATEMENT_UPDATE,
    STATEMENT_DELETE
} StatementType;

typedef struct {
    StatementType type;
    Row row_to_insert;
    Row row_to_update;
    int id_to_delete;
    ConditionColumn condition_column;
    int condition_value;
} Statement;
PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement);
ExecuteResult execute_statement(Statement* statement, Table* table);
ExecuteResult execute_insert(Statement* statement, Table* table);
ExecuteResult execute_select(Statement* statement, Table* table);
ExecuteResult execute_update(Statement* statement, Table* table);
ExecuteResult execute_delete(Statement* statement, Table* table);
void serialize_row(Row* source, void* destination);
void deserialize_row(void* source, Row* destination);
void print_row(Row* row);
void print_row_formatted(int id, const char* username, const char* email, int id_width, int username_width, int email_width);
void print_separator(int id_width, int username_width, int email_width);

#endif