#include "table.h"
#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 10

UserArray* createUserArray() {
    UserArray* userArray = malloc(sizeof(UserArray));
    if (userArray == NULL) {
        return NULL;
    }
    userArray->users = malloc(INITIAL_CAPACITY * sizeof(User));
    if (userArray->users == NULL) {
        free(userArray);
        return NULL;
    }
    userArray->count = 0;
    userArray->capacity = INITIAL_CAPACITY;
    return userArray;
}

int addUser(UserArray* userArray, int id, const char* nom) {
    if (userArray->count >= userArray->capacity) {
        int newCapacity = userArray->capacity * 2;
        User* newUsers = realloc(userArray->users, newCapacity * sizeof(User));
        if (newUsers == NULL) {
            return 0;  
        }
        userArray->users = newUsers;
        userArray->capacity = newCapacity;
    }

    userArray->users[userArray->count].id = id;
    strncpy(userArray->users[userArray->count].nom, nom, MAX_NAME_LENGTH - 1);
    userArray->users[userArray->count].nom[MAX_NAME_LENGTH - 1] = '\0';
    userArray->count++;
    return 1;  // Succès
}

void displayUsers(const UserArray* userArray) {
    for (int i = 0; i < userArray->count; i++) {
        printf("ID: %d, Nom: %s\n", userArray->users[i].id, userArray->users[i].nom);
    }
}

int saveUsersToFile(const UserArray* userArray, const char* bdd_users_file) {
    FILE* file = fopen(bdd_users_file, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 0;
    }
    for (int i = 0; i < userArray->count; i++) {
        if (fprintf(file, "%d %s\n", userArray->users[i].id, userArray->users[i].nom) < 0) {
            fclose(file);
            return 0;  // Échec de l'écriture
        }
    }
    fclose(file);
    printf("Users saved to %s.\n", bdd_users_file);
    return 1;  // Succès
}

void freeUserArray(UserArray* userArray) {
    free(userArray->users);
    free(userArray);
}