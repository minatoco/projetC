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
    char service[50];
} Login;

typedef struct{
    int id;
    char name[50];
    int role;
    char password[50];
    char service[50];
} Enregistrer;

typedef struct {
    int id;
    char title[50];
    char description[200];
    int status;
    char service[50];
    char name[50];
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

void history(){
    int champs;
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


    snprintf(query,200,"SELECT * FROM historique");
    MYSQL_RES * res;
    mysql_query(mysql, query);
    res = mysql_store_result(mysql);// dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    champs = mysql_num_rows(res);
    row = mysql_fetch_row(res);
    for (int i = 0; i < champs; ++i) {
        printf("l'utilisateur %s a %s a la date du %s\n",row[2],row[3],row[4]);
        row = mysql_fetch_row(res);
    }
    printf("\n");




    mysql_close(mysql);
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

void delete_user(int id,char name_admin[50]){
    char name_remove[50];
    char query[255];
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));

    }
    snprintf(query,255,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }
    snprintf(query,255 ,"SELECT name FROM utilisateur WHERE id = %d", id);
    MYSQL_RES * res;
    mysql_query(mysql, query);
    res = mysql_store_result(mysql);// dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    strcpy(name_remove,row[0]);

    snprintf(query,255 ,"DELETE FROM utilisateur WHERE id = %d", id);
    mysql_query(mysql, query);


    snprintf(query,255,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }

    snprintf(query,255 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (0, '%s' ,'supprimee de l''utilisateur %s')",name_admin,name_remove);
    mysql_query(mysql, query);
}

void deleteTicket(int id,char name[50]) {
    char query[255];
    MYSQL *mysql;
    mysql = mysql_init(NULL);
    mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "option");

    if (mysql_real_connect(mysql, DB_HOST, DB_USER, DB_PASSWORD, NULL, DB_PORT, NULL, 0) == NULL){
        fprintf(stderr, mysql_error(mysql));

    }
    snprintf(query,255,"USE %s",DB_NAME);
    if(mysql_query(mysql,query)){
        finish_with_error(mysql);
    }


    snprintf(query,255 ,"DELETE FROM commentaires WHERE id_ticket = %d", id);
    mysql_query(mysql, query);
    snprintf(query,255 ,"DELETE FROM ticket WHERE id = %d", id);
    mysql_query(mysql, query);
    snprintf(query,255 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (%d, '%s' ,'supprimé du ticket et des commentaires')", id,name);
    mysql_query(mysql, query);

    mysql_close(mysql);
}

void registerlogin(char name[50]){
    int service;
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

        while(1) {
            printf("Entrer le service qui a un probleme :\n");
            printf("1.Service de Production\n"
                   "2.Service de Ventes\n"
                   "3.Service de Recherche et Developpement\n"
                   "4.Service de Comptabilite et Finance\n"
                   "5.Service de Ressources Humaines\n");
            scanf("%d", &service);
            fflush(stdin);

            if (service == 1) {
                strcpy(enregistrer.service, "Service de Production");
                break;
            }
            if (service == 2) {
                strcpy(enregistrer.service, "Service de Ventes");
                break;
            }
            if (service == 3) {
                strcpy(enregistrer.service, "Service de Recherche et Developpement");
                break;
            }
            if (service == 4) {
                strcpy(enregistrer.service, "Service de Comptabilite et Finance");
                break;
            }
            if (service == 5) {
                strcpy(enregistrer.service, "Service de Ressources Humaines");
                break;
            } else {
                printf("Erreur lors du choix du service");
            }
        }

        printf("est ce que vous etes sur de ce que vous avez mis ?\necrivez oui ou non en minuscule :\n");
        inputStrings(verification,5);
        if(!strcmp(verification,"oui")){
            snprintf(query,300,"insert into utilisateur (name, role, password,service) VALUES ('%s', %d, '%s','%s')",enregistrer.name,enregistrer.role,enregistrer.password,enregistrer.service);

            mysql_query(mysql,query);
            snprintf(query,300 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (0, '%s' ,'ajout de l''utilisateur %s')",name,enregistrer.name);
            mysql_query(mysql, query);
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
                printf("Cree par : %s\n",row[5]);
                printf("Service : %s\n", row[4]);
                printf("Titre: %s\n", row[1]);
                printf("Description: %s\n", row[2]);

                if (atoi(row[3]) == 1) {
                    printf("Status: Ferme\n");
                } else if (atoi(row[3]) == 2) {
                    printf("Status: En attente de confirmation\n");
                } else{
                    printf("Status: Ouvert\n");
                }
                break;
            }
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);
}

void addcomment(int id_ticket,int id_user){
    char name[50];
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
        snprintf(query,200,"SELECT name FROM utilisateur WHERE id=%d",id_user);
        MYSQL_RES * res;
        mysql_query(mysql, query);
        res = mysql_store_result(mysql);// dernier resultat
        if(res == NULL ){
            finish_with_error(mysql);
        }
        MYSQL_ROW row;
        row = mysql_fetch_row(res);
        strcpy(name,row[0]);

        mysql_query(mysql, "SELECT * from commentaires");
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

            snprintf(query,300 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (%d, '%s' ,'ajout d''un commentaire sur le ticket %d')",id_ticket,name,id_ticket);
            mysql_query(mysql, query);
            break;
        }
    }
}

void view_ticket(int ticket_id,int id_user,int role,int status) {
    char name[50];
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
            printf("ID du commentaire : %d\n", temp);
            printf("Commentaire :%s\n", row[3]);
        }
        row = mysql_fetch_row(res);
    }// fin ajout
    mysql_free_result(res);

    while(1) {
        printf("\n");
        if(role == 0){
            printf("2. Supprimer ce ticket\n");
        }
        if(status != 1) {
            printf("1. Ajouter un commentaire\n");  // choix
        }
        printf("0. Quitter\n\n");
        scanf("%d", &choicerespons);
        fflush(stdin);
        if (choicerespons == 0) {
            return;
        }
        if(choicerespons == 2 && role == 0){
            snprintf(query,300,"USE %s",DB_NAME);
            if(mysql_query(mysql,query)){
                finish_with_error(mysql);
            }
            snprintf(query,200,"SELECT name FROM utilisateur WHERE id=%d",id_user);
            mysql_query(mysql, query);
            res = mysql_store_result(mysql);// dernier resultat
            if(res == NULL ){
                finish_with_error(mysql);
            }
            row = mysql_fetch_row(res);
            strcpy(name,row[0]);

            deleteTicket(ticket_id,name);
            return;
            }


        if(choicerespons == 1 && status!= 1){
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

                if (ticket_id == temp2 ) {
                    printf("\n");
                    print_user(temp3);
                    printf("ID du commentaire : %d\n", temp);
                    printf("Commentaire :%s\n", row[3]);
                }
                row = mysql_fetch_row(res);
            }// fin ajout
            mysql_free_result(res);

        }else{
            printf("Erreur lors du choix ");
        }


    }

}

void create_ticket(char name[50], char service_login[50]) {
    char query[300];
    char verification[5];
    int service = 0;
    Ticket ticket;

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

    while(1) {

        while (1) {

            if(!strcmp(service_login,"tout service")) {
                printf("Entrer le service qui a un probleme :\n");
                printf("1.Service de Production\n"
                       "2.Service de Ventes\n"
                       "3.Service de Recherche et Developpement\n"
                       "4.Service de Comptabilite et Finance\n"
                       "5.Service de Ressources Humaines\n");
                scanf("%d", &service);
                fflush(stdin);
            }
            if (service == 1 || !strcmp(service_login,"Service de Production")) {
                strcpy(ticket.service, "Service de Production");
                break;
            }
            if (service == 2 || !strcmp(service_login,"Service de Ventes")) {
                strcpy(ticket.service, "Service de Ventes");
                break;
            }
            if (service == 3 || !strcmp(service_login,"Service de Recherche et Developpement")) {
                strcpy(ticket.service, "Service de Recherche et Developpement");
                break;
            }
            if (service == 4 || !strcmp(service_login,"Service de Comptabilite et Finance")) {
                strcpy(ticket.service, "Service de Comptabilite et Finance");
                break;
            }
            if (service == 5 || !strcmp(service_login,"Service de Ressources Humaines")) {
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
            snprintf(query, 300, "INSERT INTO ticket(title, description, status, section, name_create) VALUES ('%s','%s',%d,'%s','%s')",
                     ticket.title,
                     ticket.description, 0,ticket.service,name);
            mysql_query(mysql, query);

            snprintf(query, 300, "SELECT id FROM ticket where title = '%s' AND description = '%s'",ticket.title,ticket.description);
            mysql_query(mysql, query);

            snprintf(query,300 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (0, '%s' ,'creation d un nouveau ticket')",name);
            mysql_query(mysql, query);
            break;
        }
    }

}

void list_users(int role,int id_user,char name[50]){
    char verification[5];
    int choice_user;
    int champs;
    char query[300];

    if(role != 0){
        return;
    }

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
    mysql_query(mysql,"SELECT * from utilisateur");
    MYSQL_RES * res;
    res = mysql_store_result(mysql); // dernier resultat
    if(res == NULL ){
        finish_with_error(mysql);
    }
    champs = mysql_num_rows(res);
    MYSQL_ROW row;
    row = mysql_fetch_row(res);
    for (int i = 0; i < champs; i++) {
        if(atoi(row[0])!= id_user) {
            printf("ID: %d\n", atoi(row[0]));
            printf("Nom d'utilisateur : %s\n", row[1]);
            if (atoi(row[2]) == 0) {
                printf("Role: SuperAdmin\n");
            } else if (atoi(row[2]) == 1) {
                printf("Role: Moderateur\n");
            } else if (atoi(row[2]) == 2) {
                printf("Role: Utilisateur\n");
            }
                if (!strcmp(row[4],"Service de Production" )) {
                    printf("Service: Service de Production\n");
                }
                if (!strcmp(row[4], "Service de Ventes")) {
                    printf("Service: Service de Ventes\n");
                }
                if (!strcmp(row[4], "Service de Recherche et Developpement")) {
                    printf("Service: Service de Recherche et Developpement\n");
                }
                if (!strcmp(row[4], "Service de Comptabilite et Finance")) {
                    printf("Service: Service de Comptabilite et Finance\n");
                }
                if (!strcmp(row[4],"Service de Ressources Humaines")) {
                    printf("Service: Service de Ressources Humaines\n");
                }
            }


        printf("\n");
        row = mysql_fetch_row(res);
    }
    mysql_free_result(res);

    while(1) {
        printf("Ecrivez l'id utilisateur que vous voulez supprimer :\n");
        printf("0. Quitter\n");
        scanf("%d", &choice_user);
        fflush(stdin);
        if (choice_user == 0) {
            return;
        } else if (choice_user != id_user) {
            printf("est ce que vous etes sur de ce que vous avez mis ?\necrivez oui ou non en minuscule :\n");
            inputStrings(verification, 5);
            if (!strcmp(verification, "oui")) {
                delete_user(choice_user,name);
            }
            list_users(role, id_user,name);
        }else
            printf("Erreur, vous ne pouvez pas vous supprimer vous meme \n\n");
            return;
    }
}

void list_tickets(int id_user,int role,char service[50],char name[50]) { // r'ajoute des ticket par rapport a la personne et au role
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
    champs = mysql_num_rows(res);
    MYSQL_ROW row;


    if(champs==0){
        printf("Vous n'avez pas de tickets \n\n");
    }else {
        row = mysql_fetch_row(res);
        for (int i = 0; i < champs; i++) {
            int temp;
            temp =atoi(row[0]);
            if(role == 0) {
                print_ticket(temp);
            }else if(role == 1 && !strcmp(service,row[4])){
                print_ticket(temp);
            }else if(role == 2 && !strcmp(name,row[5])){
                print_ticket(temp);
            }
            printf("\n");
            row = mysql_fetch_row(res);
        }
        mysql_free_result(res);
        while(1) {
            printf("Ecrivez le numero du ticket\n");
            printf("0. Quitter\n");
            scanf("%d",&choiceticket);
            fflush(stdin);
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
                int status;
                status = atoi(row[3]);
                temp = atoi(row[0]);
                if(role == 0) {
                    if (choiceticket == temp) {

                        print_ticket(temp);
                        view_ticket(temp,id_user,role,status);
                        list_tickets(id_user,role,service,name);
                        printf("Voulez-vous quitter la liste des tickets cliquez sur 0 sinon cliquez sinon\n");
                    }
                }else if(role == 1 && !strcmp(service,row[4])){
                    if (choiceticket == temp) {

                        print_ticket(temp);
                        view_ticket(temp,id_user,role,status);
                        list_tickets(id_user,role,service,name);
                        printf("Voulez-vous quitter la liste des tickets cliquez sur 0 sinon cliquez sinon\n");
                    }
                }else if(role == 2 && !strcmp(name,row[5])){
                    if (choiceticket == temp) {

                        print_ticket(temp);
                        view_ticket(temp,id_user,role,status);
                        list_tickets(id_user,role,service,name);
                        printf("Voulez-vous quitter la liste des tickets cliquez sur 0 sinon cliquez sinon\n");
                    }
                }else if (choiceticket == temp) {
                    printf("vous n'avez pas acces a ce ticket\n");
                }
                row = mysql_fetch_row(res);
            }

        }
    }

}

void update_ticket_status(int login_role, char name[50]) {
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
            printf("Entrer Nouveau status\n-0 pour ouvert \n-1 pour fermer\n-2 pour l'attente d'une confirmation: \n");
            scanf("%d", &ticket.status);
            fflush(stdin);

            if(ticket.status == 1 && login_role == 0){
                snprintf(query,200,"UPDATE ticket SET status=%d WHERE id=%d",ticket.status,id);
                mysql_query(mysql, query);
                snprintf(query,255 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (%d, '%s' ,'changer le status du ticket %d à ferme ')",id,name,id);
                mysql_query(mysql, query);
                printf("\nLes changements on bien etais pris en compte\n\n");
                mysql_free_result(res);

            }else if(ticket.status == 0 && login_role == 0){
                snprintf(query,200,"UPDATE ticket SET status=%d WHERE id=%d",ticket.status,id);
                mysql_query(mysql, query);
                snprintf(query,255 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (%d, '%s' ,'changer le status du ticket %d à Ouvert ')",id,name,id);
                mysql_query(mysql, query);
                printf("\nLes changements on bien etais pris en compte\n\n");
                mysql_free_result(res);
            }else if(ticket.status == 2){
                snprintf(query,200,"UPDATE ticket SET status=%d WHERE id=%d",ticket.status,id);
                mysql_query(mysql, query);
                snprintf(query,255 ,"INSERT INTO historique (id_ticket,user_name, action) VALUES (%d, '%s' ,'changer le status du ticket %d à En attente d'une confirmation ')",id,name,id);
                mysql_query(mysql, query);
                printf("\nLes changements on bien etais pris en compte\n\n");
                mysql_free_result(res);
            }else
                printf("Erreur lors de la saisie du nouveau status\n\n");

        }
        row = mysql_fetch_row(res);
    }
    mysql_close(mysql);
}