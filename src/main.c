#include "repl.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    Table* table = new_table();  
    if (table == NULL) {
        fprintf(stderr, "Failed to create table\n");
        return 1;
    }
    repl(table);
    free_table(table); 
    return 0;
}