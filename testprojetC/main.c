#include <MYSQL/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"
#include "function.c"


#define DB_USER "root"
#define DB_HOST "localhost"
#define DB_PASSWORD ""
#define DB_PORT 3307 // 3307 -> mariadb   3306 -> mysql
#define DB_NAME "projetc"

//la fonction changement de language

int main(int argc, char **argv) {
    char language[50];
    Login login;
    int champs;
    int choice;
    int goMenu = 0;
    int counterLogin;
    int veriflogin =0;

    read_language_from_file(language);

    do{
        MYSQL *mysql;
        mysql = mysql_init(NULL);
        mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

        if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
            fprintf(stderr, mysql_error(mysql));

        }
        char query[200];
        snprintf(query,200,"USE %s",DB_NAME);
        if(mysql_query(mysql,query)){
            finish_with_error(mysql);
        }

        for(counterLogin = 0;counterLogin<3;++counterLogin){
            if(!strcmp(language,"en") || !strcmp(language,"anglais"))
                printf("Name : ");
            else
                printf("Nom :");
            inputStrings(login.name,50);
            printf("\n");
            if(!strcmp(language,"en") || !strcmp(language,"anglais"))
                printf("Password : ");
            else
                printf("Mot de passe :");
            inputStrings(login.password,50);
            printf("\n");


            mysql_query(mysql,"SELECT * from utilisateur");
            MYSQL_RES * res;
            res = mysql_store_result(mysql); // dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            MYSQL_ROW row;
            champs = mysql_num_rows(res);

            row = mysql_fetch_row(res);

            for (int i = 0; i < champs; ++i) {

                if(!strcmp(login.name, row[1]) && !strcmp(login.password, row[3])){
                    veriflogin = 1;
                    break;
                }

                row = mysql_fetch_row(res);

            }


            if(veriflogin == 1){
                mysql_free_result(res);
                break;
            }
            mysql_free_result(res);


        }
        mysql_close(mysql);



        while (veriflogin == 1) {
            MYSQL *mysql;
            mysql = mysql_init(NULL);
            mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

            if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL) {
                fprintf(stderr, mysql_error(mysql));

            }
            char query[200];
            snprintf(query, 200, "USE %s", DB_NAME);
            if (mysql_query(mysql, query)) {
                finish_with_error(mysql);
            }


            snprintf(query, 200, "SELECT id FROM utilisateur WHERE name='%s'", login.name);
            MYSQL_RES *res;
            mysql_query(mysql, query);
            res = mysql_store_result(mysql);// dernier resultat
            if (res == NULL) {
                finish_with_error(mysql);
            }
            MYSQL_ROW row;
            row = mysql_fetch_row(res);
            login.id = atoi(row[0]);
            mysql_free_result(res);

            snprintf(query,200,"SELECT role FROM utilisateur WHERE id=%d",login.id);
            mysql_query(mysql, query);
            res = mysql_store_result(mysql);// dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            row = mysql_fetch_row(res);
            login.role = atoi(row[0]);
            mysql_free_result(res);

            snprintf(query, 200, "SELECT service FROM utilisateur WHERE id=%d", login.id);
            mysql_query(mysql, query);
            res = mysql_store_result(mysql);// dernier resultat
            if (res == NULL) {
                finish_with_error(mysql);
            }
            row = mysql_fetch_row(res);
            strcpy(login.service,row[0]);
            mysql_free_result(res);

            mysql_close(mysql);
            // role 0 , 1 et 2
            if (login.role == 0) {
                if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                    printf("1. Create a ticket\n"); //tous les roles
                    printf("2. ticket list\n"); // tous les roles
                    printf("3. change the status of a ticket\n"); // les roles 0 et 1
                    printf("4. register a user\n"); // le role 0
                    printf("5. User list\n");
                    printf("6. view history\n");
                    printf("7. Options\n");
                    printf("8. Disconnect\n"); // tous les roles
                    printf("9. Exit\n"); // tous les roles
                    printf("enter assigned number: ");
                }else {
                    printf("1. Cree un ticket\n"); //tous les roles
                    printf("2. Liste des tickets\n"); // tous les roles
                    printf("3. Changer le status d'un ticket\n"); // les roles 0 et 1
                    printf("4. Enregistrer un utilisateur\n"); // le role 0
                    printf("5. Liste des utilisateurs\n");
                    printf("6. Voir l'historique\n");
                    printf("7. Options\n");
                    printf("8. Deconnecter\n"); // tous les roles
                    printf("9. Quitter\n"); // tous les roles
                    printf("Entrer Le numero attribue: ");
                }
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n\n");

                if (choice == 1) {
                    create_ticket(login.name,login.service,language);
                } else if (choice == 2) {
                    list_tickets(login.id,login.role,login.service,login.name,language);
                } else if (choice == 3) {
                    update_ticket_status(login.role,login.name,language);
                } else if (choice == 4) {
                    registerlogin(login.name,language);
                } else if (choice == 5) {
                    list_users(login.role,login.id,login.name,language);
                } else if (choice == 6) {
                    history();
                } else if (choice == 7) {
                    change_language(language);
                    read_language_from_file(language);
                } else if (choice == 8) {
                    goMenu = 1;
                    break;
                } else if (choice == 9) {
                    goMenu = 0;
                    break;
                } else{
                    if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                        printf("Error: Invalid choice.\n");
                    }else{
                        printf("Erreur: choix invalide.\n");
                    }
                }
            }

            if(login.role == 1){
                if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                    printf("1. Create a ticket\n"); //tous les roles
                    printf("2. ticket list\n"); // tous les roles
                    printf("3. change the status of a ticket\n"); // les roles 0 et 1
                    printf("4. Options\n");
                    printf("5. Disconnect\n"); // tous les roles
                    printf("6. Exit\n"); // tous les roles
                    printf("enter assigned number: ");
                }else {
                    printf("1. Cree un ticket\n"); //tous les roles
                    printf("2. Liste des tickets\n"); // tous les roles
                    printf("3. Changer le status d'un ticket\n"); // les roles 0 et 1
                    printf("4. Options\n");
                    printf("5. Deconnecter\n"); // tous les roles
                    printf("6. Quitter\n"); // tous les roles
                    printf("Entrer Le numero attribue: ");
                }
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n\n");

                if (choice == 1) {
                    create_ticket(login.name,login.service,language);
                } else if (choice == 2) {
                    list_tickets(login.id,login.role,login.service,login.name,language);
                } else if (choice == 3) {
                    update_ticket_status(login.role,login.name,language);
                } else if (choice == 4) {
                    change_language(language);
                    read_language_from_file(language);
                } else if (choice == 5) {
                    goMenu = 1;
                    break;
                } else if (choice == 6) {
                    goMenu = 0;
                    break;
                } else{
                    if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                        printf("Error: Invalid choice.\n");
                    }else{
                        printf("Erreur: choix invalide.\n");
                    }
                }
            }

            if(login.role == 2){
                if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                    printf("1. Create a ticket\n"); //tous les roles
                    printf("2. ticket list\n"); // tous les roles
                    printf("3. Options\n");
                    printf("4. Disconnect\n"); // tous les roles
                    printf("5. Exit\n"); // tous les roles
                    printf("enter assigned number: ");
                }else {
                    printf("1. Cree un ticket\n"); //tous les roles
                    printf("2. Liste des tickets\n"); // tous les roles
                    printf("3. Options\n");
                    printf("4. Deconnecter\n"); // tous les roles
                    printf("5. Quitter\n"); // tous les roles
                    printf("Entrer Le numero attribue: ");
                }
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n\n");

                if (choice == 1) {
                    create_ticket(login.name,login.service,language);
                } else if (choice == 2) {
                    list_tickets(login.id,login.role,login.service,login.name,language);
                } else if (choice == 3) {
                    change_language(language);
                    read_language_from_file(language);
                } else if (choice == 4) {
                    goMenu = 1;
                    break;
                } else if (choice == 5) {
                    goMenu = 0;
                    break;
                } else {
                    if(!strcmp(language,"en") || !strcmp(language,"anglais")){
                        printf("Error: Invalid choice.\n");
                    }else{
                        printf("Erreur: choix invalide.\n");
                    }
                }
            }
        }
    }
    while(goMenu == 1);
    return 0;
}
