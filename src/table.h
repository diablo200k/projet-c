#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>

#define MAX_NAME_LENGTH 50

typedef struct {
    int id;
    char nom[MAX_NAME_LENGTH];
} User;

typedef struct {
    User* users;
    int count;
    int capacity;
} UserArray;

UserArray* createUserArray();
int addUser(UserArray* userArray, int id, const char* nom);
void displayUsers(const UserArray* userArray);
int saveUsersToFile(const UserArray* userArray, const char* bdd_users_file);
void freeUserArray(UserArray* userArray);

#endif // TABLE_H