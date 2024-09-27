#include "table.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>



Table* new_table() {
    Table* table = malloc(sizeof(Table));
    table->num_rows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }

    // Charger les données du fichier
    FILE* file = fopen("database_backup.txt", "r");
    if (file != NULL) {
        char line[512];
        while (fgets(line, sizeof(line), file)) {
            Row row;
            if (sscanf(line, "%d,%[^,],%s", &row.id, row.username, row.email) == 3) {
                void* page = row_slot(table, table->num_rows);
                if (page != NULL) {
                    serialize_row(&row, page);
                    table->num_rows++;
                }
            }
        }
        fclose(file);
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