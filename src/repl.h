#ifndef REPL_H
#define REPL_H

#include "table.h"
#include "input.h"  

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
} MetaCommandResult;

MetaCommandResult do_meta_command(InputBuffer* input_buffer, Table* table); 
void repl(Table* table);

#endif