#include <stdio.h>
#include <string.h>


typedef struct {
    int id;
    char name[50];
    int role;
    char password[50];
} Login;

typedef struct {
    int id;
    char title[50];
    char description[200];
    int status;
    char service[50];
} Ticket;

typedef struct {
    int id_comment; //id_utilisateur
    int id_ticket;
    char comment[400];
}Comment;

Comment commentaire[100];
Ticket tickets[100];
int ticket_count = 0;
int commentaire_count=0;


void inputStrings(char *string,int size){
    fgets(string,size,stdin);
    if(string[strlen(string)-1] == '\n'){
        string[strlen(string)-1] = '\0';
    }else{
        fflush(stdin);
    }
}

void print_ticket(Ticket ticket) {


    printf("ID: %d\n", ticket.id);
    printf("Service : %s\n",ticket.service);
    printf("Titre: %s\n", ticket.title);
    printf("Description: %s\n", ticket.description);

    if(ticket.status == 1){
        printf("Status: Ferme\n");
    }else{
        printf("Status: Ouvert\n");
    }

}

void addcomment(int id_ticket){
    Comment comments;
    comments.id_ticket=id_ticket;
    comments.id_comment=commentaire_count+1;

    printf("veuillez saisir votre commentaire:\n");
    inputStrings(comments.comment,400);


    commentaire[commentaire_count++] = comments;
}

void view_ticket(int ticket_id) {
    int choicerespons;

    Comment comments;
    //ajoute des ancien commentaire
    for (int i = 0; i < commentaire_count; ++i) {
        if (ticket_id == commentaire[i].id_ticket) {
            printf("\n");
            printf("ID du ticket : %d\n", commentaire[i].id_ticket);
            printf("ID du commentaire : %d\n", commentaire[i].id_comment);
            printf("Commentaire :%s\n", commentaire[i].comment);
        }
    }// fin ajout
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
            addcomment(ticket_id);
            print_ticket(tickets[ticket_id-1]);
            for (int i = 0; i < commentaire_count; ++i) {
                if (ticket_id == commentaire[i].id_ticket) {
                    printf("\n\n");
                    printf("ID du ticket : %d\n", commentaire[i].id_ticket);
                    printf("ID du commentaire : %d\n", commentaire[i].id_comment);
                    printf("Commentaire :%s\n", commentaire[i].comment);
                }
            }

        }else{
            printf("Erreur lors du choix ");
        }


    }

}

void create_ticket() {
    if (ticket_count >= 100) {
        printf("Erreur: Impossible de creer un nouveau ticket. Numbre maximun de tickets créable atteint.\n");
        return;
    }
    int service;
    Ticket ticket;
    ticket.id = ticket_count + 1;

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
    inputStrings(ticket.title,50);
    printf("\n");

    printf("Entrer la description du ticket:");
    inputStrings(ticket.description,200);
    printf("\n");
    ticket.status = 0;

    tickets[ticket_count++] = ticket;

}

void list_tickets() {
    int choiceticket;
    if(ticket_count==0){
        printf("Vous n'avez pas de tickets \n\n");

    }else {
        for (int i = 0; i < ticket_count; i++) {
            print_ticket(tickets[i]);
            printf("\n\n");
        }
        while(1) {
            printf("Ecrivez le numero du ticket\n");
            printf("0. Quitter\n");
            scanf("%d",&choiceticket);
            if(choiceticket == 0) {
                return;
            }
                for (int i = 0; i < ticket_count; ++i) {
                    if (choiceticket == tickets[i].id) {
                        print_ticket(tickets[choiceticket-1]);
                        view_ticket(tickets[i].id);
                        list_tickets();
                    }
                }

        }
    }

}

void update_ticket_status() {
    int id;
    printf("Entrer ID du ticket: ");
    scanf("%d", &id);

    for (int i = 0; i < ticket_count; i++) {
        if (tickets[i].id == id) {
            printf("Entrer Nouveau status(0 pour ouvert et 1 pour ferme): ");
            scanf("%d", &tickets[i].status);

            if(tickets[i].status == 1){
                printf("Ferme\n\n");
            }else{
                printf("Ouvert\n\n");
            }
            return;
        }
    }

    printf("Erreur: Ticket introuvable.\n");
}

int main() {
        Ticket ticket;
        Login login;
        int choice;
        int goMenu = 1;
        int counterLogin;

       do{

        for(counterLogin = 0;counterLogin<3;++counterLogin){

            printf("Name : ");
            inputStrings(login.name,50);
            printf("\n");
            printf("Mot de passe :");
            inputStrings(login.password,50);
            printf("\n\n");

            if(!strcmp(login.name, "admin") && !strcmp(login.password, "admin")){
                counterLogin = 0;
                break;
            }
        }



    while (!strcmp(login.name, "admin") && !strcmp(login.password, "admin")){

        printf("1. Cree un ticket\n");
        printf("2. Liste des tickets\n");
        printf("3. Changer le status d'un ticket\n");
        printf("4. Deconnecter\n");
        printf("5. Quitter\n");
        printf("Entrer Le numero attribue: ");
        scanf("%d", &choice);
        fflush(stdin);
        printf("\n\n");

        if (choice == 1) {
            create_ticket();
        } else if (choice == 2) {
            list_tickets();
        } else if (choice == 3) {
            update_ticket_status();
        } else if (choice == 5) {
            goMenu=0;
            break;
        } else if (choice == 4) {
            return 0;
        } else
            printf("Error: Invalid choice.\n");
        }
    }
       while(goMenu == 1);

    return 0;
}