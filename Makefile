CC=gcc
CFLAGS=-Wall -Werror


SRC_DIR=src

all: program


program: $(SRC_DIR)/main.o $(SRC_DIR)/btree.o $(SRC_DIR)/table.o $(SRC_DIR)/repl.o $(SRC_DIR)/input.o $(SRC_DIR)/statement.o
	$(CC) $(CFLAGS) -o program $^

$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/btree.o: $(SRC_DIR)/btree.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/table.o: $(SRC_DIR)/table.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/repl.o: $(SRC_DIR)/repl.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/input.o: $(SRC_DIR)/input.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/statement.o: $(SRC_DIR)/statement.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o program
