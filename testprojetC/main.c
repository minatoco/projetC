#include <MYSQL/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DB_USER "root"
#define DB_HOST "localhost"
#define DB_PASSWORD ""
#define DB_PORT 3307 // 3307 -> mariadb   3306 -> mysql
#define DB_NAME "projetc"

typedef struct {
    int id;
    char name[50];
    int role;
    char password[50];
} Login;

typedef struct{
    int id;
    char name[50];
    int role;
    char password[50];
} Enregistrer;

typedef struct {
    int id;
    char title[50];
    char description[200];
    int status;
    char service[50];
} Ticket;

typedef struct {
    int id_user; //id_utilisateur
    int id_ticket;
    char comment[400];
}Comment;




void finish_with_error(MYSQL *mysql)
{
    fprintf(stderr, "%s\n", mysql_error(mysql));
    mysql_close(mysql);
    exit(1);
}

void inputStrings(char *string,int size){
    fgets(string,size,stdin);
    if(string[strlen(string)-1] == '\n'){
        string[strlen(string)-1] = '\0';
    }else{
        fflush(stdin);
    }
}

void print_user(int id_utilisateur){
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


    snprintf(query,200,"SELECT name FROM utilisateur WHERE id=%d",id_utilisateur);
    MYSQL_RES * res;
    mysql_query(mysql, query);
    res = mysql_store_result(mysql);// dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    printf("nom d'utilisateur :%s\n",row[0]);



    mysql_close(mysql);
}

void registerlogin(){
    char query[300];
    char verification[3];
    Enregistrer enregistrer;

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");


    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
    fprintf(stderr, mysql_error(mysql));
    }


    while(1) {
        snprintf(query,300,"USE %s",DB_NAME);
        if(mysql_query(mysql,query)){
            finish_with_error(mysql);
        }
        mysql_query(mysql,"SELECT * from utilisateur");
        MYSQL_RES * res;
        res = mysql_store_result(mysql); // dernier resultat
        if(res == NULL ){
            finish_with_error(mysql);
        }

        printf("Choisissez le name de la personne :\n");
        inputStrings(enregistrer.name, 50);
        printf("Choisissez le mot de passe :\n");
        inputStrings(enregistrer.password, 50);
        printf("choisissez le role qui l'aura:\n 0 = admin\n1 = moderateur\n2 = utilisateur\n");
        scanf("%d", &enregistrer.role);
        fflush(stdin);

        printf("est ce que vous etes sur de ce que vous avez mis ?\necrivez oui ou non en minuscule :\n");
        inputStrings(verification,5);
        if(!strcmp(verification,"oui")){
            snprintf(query,300,"insert into utilisateur (name, role, password) VALUES ('%s', %d, '%s')",enregistrer.name,enregistrer.role,enregistrer.password);
            printf("%s\n",query);
            mysql_query(mysql,query);
            break;
        }

    }

}

void print_ticket(int id_ticket) {
    int champs;
    char query[300];
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");


    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));
    }
    snprintf(query,300,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }
    mysql_query(mysql,"SELECT * from ticket");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    champs = mysql_num_rows(res);
    row = mysql_fetch_row(res);
    for (int i = 0; i < champs ; ++i) {
        int temp;
        temp = atoi(row[0]);
            if(temp == id_ticket) {
                printf("ID: %d\n", temp);
                printf("Service : %s\n", row[4]);
                printf("Titre: %s\n", row[1]);
                printf("Description: %s\n", row[2]);

                if (atoi(row[3]) == 1) {
                    printf("Status: Ferme\n");
                } else {
                    printf("Status: Ouvert\n");
                }
                break;
            }
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
}

void addcomment(int id_ticket,int id_user){
    char query[300];
    char verification[3];
    Comment comments;

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));
    }

    while(1) {
        snprintf(query, 300, "USE %s", DB_NAME);
        if (mysql_query(mysql, query)) {
            finish_with_error(mysql);
        }
        mysql_query(mysql, "SELECT * from commentaires");
        MYSQL_RES *res;
        res = mysql_store_result(mysql); // dernier resultat
        if (res == NULL) {
            finish_with_error(mysql);
        }
        printf("veuillez saisir votre commentaire:\n");
        inputStrings(comments.comment, 400);



        printf("est ce que vous etes sur de ce que vous avez mis ?\necrivez oui sinon ecrivez n'importe quoi :\n");
        inputStrings(verification,5);
        if(!strcmp(verification,"oui")){
            snprintf(query,300,"insert into commentaires (id_utilisateur, id_ticket, commentaire) VALUES (%d, %d, '%s')",id_user,id_ticket,comments.comment);
            mysql_query(mysql,query);
            break;
        }
    }
}

void view_ticket(int ticket_id,int id_user) {
    int choicerespons;
    int champs;
    char query[300];
    char verification[3];
    Comment comments;

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");



    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));
    }

    snprintf(query,300,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }
    mysql_query(mysql,"SELECT * from commentaires");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }

    MYSQL_ROW row;
    champs = mysql_num_rows(res);
    row = mysql_fetch_row(res);

    //ajoute des ancien commentaire
    for (int i = 0; i < champs; ++i) {
        int temp;
        int temp2;
        int temp3;
        temp3 = atoi(row[1]);
        temp2 = atoi(row[2]);
        temp = atoi(row[0]);
        if (ticket_id == temp2 ) {
            printf("\n");
            print_user(temp3);
            printf("ID du ticket : %d\n", temp2);
            printf("ID du commentaire : %d\n", temp);
            printf("Commentaire :%s\n", row[3]);
            row = mysql_fetch_row(res);
        }
    }// fin ajout
    mysql_free_result(res);

    while(1) {
        printf("\n");
        printf("1. Ajouter un commentaire\n");  // choix
        printf("0. Quitter\n\n");
        scanf("%d", &choicerespons);
        fflush(stdin);
        if (choicerespons == 0) {
            return;
        }
        if(choicerespons == 1){
            addcomment(ticket_id,id_user);
            print_ticket(ticket_id);

            mysql_query(mysql,"SELECT * from commentaires");
            res = mysql_store_result(mysql); // dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            champs = mysql_num_rows(res);
            row = mysql_fetch_row(res);
            for (int i = 0; i < champs; ++i) {
                int temp3;
                int temp;
                int temp2;
                temp3 = atoi(row[1]);
                temp2 = atoi(row[2]);
                temp = atoi(row[0]);

                printf("%d,%d",ticket_id, temp);
                if (ticket_id == temp2 ) {
                    printf("\n");
                    print_user(temp3);
                    printf("ID du ticket : %d\n", temp2);
                    printf("ID du commentaire : %d\n", temp);
                    printf("Commentaire :%s\n", row[3]);
                    row = mysql_fetch_row(res);
                }
            }// fin ajout
            mysql_free_result(res);

        }else{
            printf("Erreur lors du choix ");
        }


    }

}

void create_ticket() {
    int champs;
    char query[300];
    char verification[5];

    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");


    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));
    }

    int service;
    Ticket ticket;

    snprintf(query,300,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }
    mysql_query(mysql,"SELECT * from ticket");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }

    while(1) {

        while (1) {


            printf("Entrer le service qui a un probleme :\n");
            printf("1.Service de Production\n"
                   "2.Service de Ventes\n"
                   "3.Service de Recherche et Developpement\n"
                   "4.Service de Comptabilite et Finance\n"
                   "5.Service de Ressources Humaines\n");
            scanf("%d", &service);
            fflush(stdin);

            if (service == 1) {
                strcpy(ticket.service, "Service de Production");
                break;
            }
            if (service == 2) {
                strcpy(ticket.service, "Service de Ventes");
                break;
            }
            if (service == 3) {
                strcpy(ticket.service, "Service de Recherche et Developpement");
                break;
            }
            if (service == 4) {
                strcpy(ticket.service, "Service de Comptabilite et Finance");
                break;
            }
            if (service == 5) {
                strcpy(ticket.service, "Service de Ressources Humaines");
                break;
            } else {
                printf("Erreur sur le lieu u probleme\n\n");
            }
        }

        printf("\n");
        printf("Entrer le titre du ticket:");
        inputStrings(ticket.title, 50);
        printf("\n");

        printf("Entrer la description du ticket:");
        inputStrings(ticket.description, 200);
        printf("\n");

        printf("est ce que vous etes sur de ce que vous avez mis ?\necrivez oui ou non en minuscule :\n");
        inputStrings(verification, 5);
        if (!strcmp(verification, "oui")) {
            snprintf(query, 300, "INSERT INTO ticket(title, description, status, section) VALUES ('%s','%s',%d,'%s')",
                     ticket.title,
                     ticket.description, 0,ticket.service);
            mysql_query(mysql, query);
            break;
        }
    }

}

void list_tickets(int id_user) {
    int champs;
    int choiceticket;

    char query[300];


    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");


    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));
    }

    snprintf(query,300,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }
    mysql_query(mysql,"SELECT * from ticket");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    champs = mysql_num_rows(res);



    if(champs==0){
        printf("Vous n'avez pas de tickets \n\n");

    }else {
        row = mysql_fetch_row(res);
        for (int i = 0; i < champs; i++) {
            int temp;
            temp =atoi(row[0]);

           print_ticket(temp);
            printf("\n\n");
            row = mysql_fetch_row(res);
        }
        mysql_free_result(res);
        while(1) {
            printf("Ecrivez le numero du ticket\n");
            printf("0. Quitter\n");
            scanf("%d",&choiceticket);
            if(choiceticket == 0) {
                return;
            }

            mysql_query(mysql,"SELECT * from ticket");
            res = mysql_store_result(mysql); // dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            MYSQL_ROW row;
            champs = mysql_num_rows(res);
            row = mysql_fetch_row(res);
            for (int i = 0; i < champs; ++i) {
                int temp;
                temp = atoi(row[0]);
                if (choiceticket == temp) {

                    print_ticket(temp);
                    view_ticket(temp,id_user);
                    list_tickets(id_user);
                    printf("Voulez-vous quitter la liste des tickets cliquez sur 0 sinon cliquez sinon\n");
                }
                row = mysql_fetch_row(res);
            }

        }
    }

}

void update_ticket_status() {
    int id;
    int champs;
    Ticket ticket;
    printf("Entrer ID du ticket que vous voulez fermer: ");
    scanf("%d", &id);
    fflush(stdin);

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

    mysql_query(mysql,"SELECT * from ticket");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    champs = mysql_num_rows(res);

    row = mysql_fetch_row(res);
    for (int i = 0; i < champs; i++) {

        int temp;
        temp = atoi(row[0]);
        if (temp == id) {
            printf("Entrer Nouveau status(\n-0 pour ouvert \n-1 pour fermer): \n");
            scanf("%d", &ticket.status);
            fflush(stdin);

            if(ticket.status == 1){
                snprintf(query,200,"UPDATE ticket SET status=%d WHERE id=%d",ticket.status,id);
                mysql_query(mysql, query);
                printf("\nLes changements on bien etais pris en compte\n\n");
                mysql_free_result(res);

            }else if(ticket.status == 0){
                snprintf(query,200,"UPDATE ticket SET status=%d WHERE id=%d",ticket.status,id);
                mysql_query(mysql, query);
                printf("\nLes changements on bien etais pris en compte\n\n");
                mysql_free_result(res);
            }else{
                printf("Erreur Veuillez ressaisir le nouveau status\n\n");
            }

        }
        row = mysql_fetch_row(res);
    }
    mysql_close(mysql);
}

int main(int argc, char **argv) {
    Login login;
    int champs;
    int choice;
    int goMenu = 0;
    int counterLogin;
    int veriflogin =0;
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


    do{

        for(counterLogin = 0;counterLogin<3;++counterLogin){
            printf("Name : ");
            inputStrings(login.name,50);
            printf("\n");
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
                break;
            }
            mysql_free_result(res);


        }
        mysql_close(mysql);



        while (veriflogin == 1){

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


            snprintf(query,200,"SELECT id FROM utilisateur WHERE name='%s'",login.name);
            MYSQL_RES * res;
            mysql_query(mysql, query);
            res = mysql_store_result(mysql);// dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            MYSQL_ROW row;
            row = mysql_fetch_row(res);
            login.id = atoi(row[0]);

            mysql_close(mysql);

            printf("1. Cree un ticket\n");
            printf("2. Liste des tickets\n");
            printf("3. Changer le status d'un ticket\n");
            printf("4. Enregistrer un utilisateur\n");
            printf("5. Deconnecter\n");
            printf("6. Quitter\n");
            printf("Entrer Le numero attribue: ");
            scanf("%d", &choice);
            fflush(stdin);
            printf("\n\n");

            if (choice == 1) {
                create_ticket();
            } else if (choice == 2) {
                list_tickets(login.id);
            } else if (choice == 3) {
                update_ticket_status();
            } else if(choice == 4){
                registerlogin();
            } else if (choice == 5) {
                goMenu=1;
                break;
            } else if (choice == 6) {
                goMenu=0;
                break;
            } else
                printf("Error: Invalid choice.\n");
        }
    }
    while(goMenu == 1);
    return 0;
}
