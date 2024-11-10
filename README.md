# Projet de Base de Données Simple en C

## Description
Ce projet implémente une base de données simple en C, offrant des fonctionnalités de base pour stocker, récupérer, mettre à jour et supprimer des données. Il utilise un système de stockage basé sur des fichiers texte pour la persistance des données et une structure en mémoire pour des opérations rapides.

## Fonctionnalités
- **Opérations CRUD** (Create, Read, Update, Delete) sur les enregistrements
- **Interface en ligne de commande (CLI)** pour interagir avec la base de données
- **Stockage persistant** des données dans un fichier texte `database_backup.txt`
- **Gestion efficace de la mémoire** avec une structure de table paginée
- **Affichage formaté** des résultats de requêtes
- **Indexation des données** utilisant une structure d'arbre B

## Structure du Projet
- `src/btree.c` et `btree.h` : Implémentation de l'arbre B pour l'indexation
- `src/input.c` et `input.h` : Gestion des entrées utilisateur
- `src/repl.c` et `repl.h` : Boucle Read-Eval-Print pour l'interface utilisateur
- `src/statement.c` et `statement.h` : Traitement des requêtes SQL
- `src/table.c` et `table.h` : Gestion de la structure de la table en mémoire
- `src/main.c` : Point d'entrée du programme
- `Makefile` : Script de compilation du projet

## Compilation et Exécution
Le projet utilise un Makefile pour simplifier la compilation. Suivez ces étapes :
1. Assurez-vous d'avoir GCC installé sur votre système.
2. Ouvrez un terminal et naviguez jusqu'au répertoire du projet.
3. Pour compiler le projet, exécutez :
   ```bash
   make
Pour exécuter le programme, utilisez :
   bash
   ./program

Pour nettoyer les fichiers objets et l'exécutable, utilisez :
   bash
   make clean

## Utilisation
Après avoir lancé le programme, vous pouvez utiliser les commandes suivantes :

- `insert <id> <username> <email>` : Insérer un nouvel enregistrement avec un identifiant, un nom d'utilisateur, et une adresse email.
- `select` : Afficher tous les enregistrements dans la base de données.
- `select where id = <id>` : Afficher l'enregistrement spécifique ayant l'identifiant indiqué.
- `update <id> set username=<username> email=<email>` : Mettre à jour le nom d'utilisateur et l'email pour l'enregistrement avec l'identifiant indiqué.
- `delete where id = <id>` : Supprimer l'enregistrement correspondant à l'identifiant indiqué.
- `.exit` : Quitter le programme.
- `.help` : Afficher l'aide avec la liste des commandes disponibles.

## Limitations
- **Taille maximale de la table** : 10 000 enregistrements (100 pages * 100 lignes par page).
- **Taille maximale du nom d'utilisateur** : 32 caractères.
- **Taille maximale de l'email** : 255 caractères.

## Structure des Données
Chaque enregistrement contient :
- **ID** (entier)
- **Nom d'utilisateur** (32 caractères max)
- **Adresse email** (255 caractères max)

Les données sont stockées en mémoire dans une structure de table paginée pour une gestion efficace, et un **arbre B** est utilisé pour l'indexation, permettant des recherches rapides.

## Améliorations Futures
- Ajout de fonctionnalités de recherche plus avancées.
- Implémentation de transactions et de verrouillage de fichiers.
- Optimisation des performances pour de grandes quantités de données.
- Support de types de données supplémentaires.

## Auteur
Hassan Mohamed

