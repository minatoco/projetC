void finish_with_error(MYSQL *mysql);

void inputStrings(char *string,int size);

void history();

void print_user(int id_utilisateur);

void delete_user(int id,char name_admin[50]);

void deleteTicket(int id,char name[50]);

void registerlogin(char name[50]);

void print_ticket(int id_ticket);

void addcomment(int id_ticket,int id_user);

void view_ticket(int ticket_id,int id_user,int role,int status);

void create_ticket(char name[50], char service_login[50]);

void list_users(int role,int id_user,char name[50]);

void list_tickets(int id_user,int role,char service[50],char name[50]);

void update_ticket_status(int login_role, char name[50]);