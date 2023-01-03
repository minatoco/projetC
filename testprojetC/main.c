#include <stdio.h>
#include <string.h>


typedef struct {
    int id;
    int role;
    char name[50];
    char password[50];
} Login;

typedef struct {
    int id;
    char title[50];
    char description[150];
    int status;
} Ticket;

Ticket tickets[100];
int ticket_count = 0;

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
    printf("Titre: %s\n", ticket.title);
    printf("Description: %s\n", ticket.description);
    if(ticket.status == 1){
        printf("Status: Ferme\n");
    }else{
        printf("Status: Ouvert\n");
    }
}

void create_ticket() {
    if (ticket_count >= 100) {
        printf("Erreur: Impossible de creer un nouveau ticket. Numbre maximun de tickets créable atteint.\n");
        return;
    }

    Ticket ticket;
    ticket.id = ticket_count + 1;

    char status;

    printf("\n");
    printf("Entrer le titre du ticket:");
    inputStrings(ticket.title,50);
    printf("\n");

    printf("Entrer la description du ticket:");
    inputStrings(ticket.description,200);

    ticket.status = 0;

    tickets[ticket_count++] = ticket;

}

void list_tickets() {
    if(ticket_count==0){
        printf("Vous n'avez pas de tickets \n\n");

    }else {
        for (int i = 0; i < ticket_count; i++) {
            print_ticket(tickets[i]);
            printf("\n\n");
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
            scanf("%d", tickets[i].status);

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


        Login login;
        printf("Bonjour Connecter Vous\n");
        printf("Name : ");
        inputStrings(login.name,50);
        printf("\n");
        printf("Mot de passe :");
        inputStrings(login.password,50);



    while (login.name!="admin" && login.password!="admin"){
        int choice;
        printf("1. Cree un ticket\n");
        printf("2. Liste des tickets\n");
        printf("3. Changer le status d'un ticket\n");
        printf("4. Quitter\n");
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
        } else if (choice == 4) {
            break;
        } else {
            printf("Error: Invalid choice.\n");
        }
    }
    return 0;
}