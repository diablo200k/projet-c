#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// donner de user 

typedef struct {
    int id ;
    char nom [50]; 
    char email[100];
    char mdp[50];
    int age;

}User;


void addUser(User *users, int *count,int id,char *nom,char *email, char *mdp, int age){
    users[*count].id = id;
    strcpy(users[*count].nom, nom);
    strcpy(users[*count].email, email);
    strcpy(users[*count].mdp, mdp);
    users[*count].age = age;
    (*count)++;

}


//afficher les users

void displayusers(User *users, int count){
    for (int i = 0 ; i< count ; i++){
        printf("ID: %d, Nom: %s, Email: %s, Age: %d\n", users[i].id, users[i].nom, users[i].email, users[i].age);
    }
}


// save user 


void saveusertofile(User *users, int count , const char *bdd_users_file){
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    for (int i=0;i<count;i++){
        fprintf(file, "%d %s %s %s %d\n", users[i].id, users[i].nom, users[i].email, users[i].mdp, users[i].age);
    }
    fclose(file);
    printf("user saved at %s.\n ",bdd_users_file)
}