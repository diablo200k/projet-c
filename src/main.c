#include "table.h"

int main() {
    UserArray* userArray = createUserArray();
    if (userArray == NULL) {
        printf("Failed to create user array\n");
        return 1;
    }

    addUser(userArray, 1, "Alice");
    addUser(userArray, 2, "Bob");

    displayUsers(userArray);

    if (!saveUsersToFile(userArray, "users.txt")) {
        printf("Failed to save users to file\n");
    }

    freeUserArray(userArray);
    return 0;
}