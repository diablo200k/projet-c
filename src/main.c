#include <stdio.h>
#include <stdlib.h>
#include "repl.c"
#include "table.c"
#include <stdbool.h>
#include <string.h>


int main(){
  int max_users=10;
  int count=0


  User *users =malloc(max_users * sizeof(User));

  addUser(users, &count, 1, "Alice", "alice@example.com", "password123", 25);
  addUser(users, &count, 2, "Bob", "bob@example.com", "password456", 30);
  
  displayusers(users, count);
  saveusertofile(users, count, "users.txt")
  free(users);
  returnà;
}
