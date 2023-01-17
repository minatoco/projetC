void finish_with_error(MYSQL *mysql);

void inputStrings(char *string,int size);

void history();

void print_user(int id_utilisateur,char language[50]);

void delete_user(int id,char name_admin[50]);

void deleteTicket(int id,char name[50]);

void registerlogin(char name[50],char language[50]);

void print_ticket(int id_ticket,char language[50]);

void addcomment(int id_ticket,int id_user,char language[50]);

void view_ticket(int ticket_id,int id_user,int role,int status,char language[50]);

void create_ticket(char name[50], char service_login[50],char language[50]);

void list_users(int role,int id_user,char name[50],char language[50]);

void list_tickets(int id_user,int role,char service[50],char name[50],char language[50]);

void update_ticket_status(int login_role, char name[50],char language[50]);

void read_language_from_file(char *language);

void change_language(char *new_language);