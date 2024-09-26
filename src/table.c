#include "table.h"
#include <stdlib.h>
#include <string.h>

Table* new_table() {
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }
    return table;
}

void free_table(Table* table) {
    for (int i = 0; table->pages[i]; i++) {
        free(table->pages[i]);
    }
    free(table);
}

void* row_slot(Table* table, uint32_t row_num) {
    uint32_t page_num = row_num / ROWS_PER_PAGE;
    void* page = table->pages[page_num];
    if (page == NULL) {
        
        page = table->pages[page_num] = malloc(ROWS_PER_PAGE * sizeof(Row));
    }
    uint32_t row_offset = row_num % ROWS_PER_PAGE;
    return page + (row_offset * sizeof(Row));
}