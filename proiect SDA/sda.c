#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RON, EUR, GBP } Currency;

typedef struct account {
    struct account* urm;
    Currency currency;
    float sold;
    char iban[24];
} account;

typedef struct User {
    struct User* urm;
    struct account* headAccount;
    struct account* tailAccount;
    char username[20], cnp[14], password[20];
} User;

const char* currencyToString(Currency currency) {
    switch (currency) {
    case RON: return "RON";
    case EUR: return "EUR";
    case GBP: return "GBP";
    default: return "undefined";
    }
}

Currency stringToCurrency(const char* currency) {
    if (strcmp("RON", currency) == 0) return RON;
    if (strcmp("EUR", currency) == 0) return EUR;
    if (strcmp("GBP", currency) == 0) return GBP;
    return RON;
}

account* SearchAccount(account* head, const char* iban) {
    account* q = head;
    while (q) {
        if (strcmp(q->iban, iban) == 0) return q;
        q = q->urm;
    }
    return NULL;
}

User* AddToEndAccount(User* user, account* p) {
    if (user->headAccount == NULL) {
        user->headAccount = p;
        user->tailAccount = p;
    }
    else {
        user->tailAccount->urm = p;
        user->tailAccount = p;
    }
    return user;
}

void RemoveAllAccounts(account* head) {
    account* aux;
    while (head) {
        aux = head;
        head = head->urm;
        free(aux);
    }
}

void ShowAccounts(account* headAccount) {
    if (headAccount == NULL) {
        printf("No accounts found.\n");
        return;
    }
    account* q = headAccount;
    printf("--------------------------------------\n");
    while (q) {
        printf("Iban = %s currency =  %s, sold = %.2f\n", q->iban, currencyToString(q->currency), q->sold);
        q = q->urm;
    }
    printf("--------------------------------------\n");
}

User* SearchUserByUsername(User* head, const char* username) {
    User* q = head;
    while (q) {
        if (strcmp(q->username, username) == 0) return q;
        q = q->urm;
    }
    return NULL;
}

User* SearchUserByCNP(User* head, const char* cnp) {
    User* q = head;
    while (q) {
        if (strcmp(q->cnp, cnp) == 0) return q;
        q = q->urm;
    }
    return NULL;
}

User* AddSortedUser(User* head, User* p) {
    User* q = head;
    if (head == NULL) {
        head = p;
    }
    else if (strcmp(head->username, p->username) > 0) {
        p->urm = head;
        head = p;
        return head;
    }
    else {
        while (q->urm != NULL && strcmp(q->urm->username, p->username) < 0) {
            q = q->urm;
        }
        p->urm = q->urm;
        q->urm = p;
    }
    return head;
}

User* RemoveUser(User* head, char username[]) {
    User* aux = NULL, * q = NULL;
    if (head == NULL) {
        return NULL;
    }
    else if (strcmp(head->username, username) == 0) {
        aux = head;
        head = head->urm;
        RemoveAllAccounts(aux->headAccount);
        free(aux);
        return head;
    }
    else {
        q = head;
        while (q->urm != NULL && strcmp(q->urm->username, username) != 0) {
            q = q->urm;
        }
        if (q->urm == NULL) {
            return NULL;
        }
        else {
            aux = q->urm;
            q->urm = q->urm->urm;
            RemoveAllAccounts(aux->headAccount);
            free(aux);
        }
        return head;
    }
}

void ShowUsers(User* head) {
    User* p = head;
    while (p != NULL) {
        printf("username = %s\nCNP = %s\n", p->username, p->cnp);
        ShowAccounts(p->headAccount);
        p = p->urm;
    }
}

User* ReadUsersFromFile(const char* filePath) {
    User* p, * q = NULL;
    User* head = NULL;
    FILE* f = fopen(filePath, "r");
    if (f == NULL) {
        printf("The users file could not be opened");
        return NULL;
    }
    while (!feof(f)) {
        p = (User*)malloc(sizeof(User));
        if (!p) {
            printf("Memory allocation failed!\n");
            break;
        }
        if (fscanf(f, "%19s %19s %13s", p->username, p->password, p->cnp) != 3) {
            free(p);
            break;
        }
        p->urm = NULL;
        p->headAccount = NULL;
        p->tailAccount = NULL;
        q = SearchUserByCNP(head, p->cnp);
        if (q || feof(f) || SearchUserByUsername(head, p->username)) {
            free(p);
        }
        else {
            head = AddSortedUser(head, p);
        }
    }
    fclose(f);
    return head;
}


void ReadAccountsFromFile(User* head, const char* filePath) {
    User* q = NULL;
    account* aux = NULL, * temp = NULL;
    char cnp[14], currency[4];
    FILE* f = fopen(filePath, "r");
    if (f == NULL) {
        printf("The account file could not be opened");
        return;
    }
    while (!feof(f)) {
        aux = (account*)malloc(sizeof(account));
        if (!aux) {
            printf("Memory allocation failed!\n");
            break;
        }
        if (fscanf(f, "%13s %34s %3s %f", cnp, aux->iban, currency, &aux->sold) != 4) {
            free(aux);
            break;
        }
        aux->currency = stringToCurrency(currency);
        aux->urm = NULL;
        q = SearchUserByCNP(head, cnp);
        if (!q) {
            free(aux);
            printf("The user with CNP %s was not found\n", cnp);
            continue;
        }
        else {
            temp = SearchAccount(q->headAccount, aux->iban);
            if (temp) {
                printf("The account with IBAN %s already exists\n", aux->iban);
                free(aux);
            }
            else {
                AddToEndAccount(q, aux);
            }
        }
    }
    fclose(f);
}


void AdminMenu() {
    printf("\n---Admin Menu---\n");
    printf("1. Add User\n");
    printf("2. Add IBAN code\n");
    printf("3. Display users\n");
    printf("4. Delete IBAN code\n");
    printf("5. Edit user data according to CNP\n");
    printf("6. Change password\n");
    printf("7. Sign OFF\n");
}

void UserMenu() {
    printf("\n---User Menu---\n");
    printf("1. Account selection for operations\n");
    printf("2. Display extracts\n");
    printf("3. Transfer money\n");
    printf("4. Pay utility bills\n");
    printf("6. Change password\n");
    printf("7. Sign OFF\n");
}

void addUser(User** head) {
    FILE* file = fopen("users.txt", "a");
    if (file == NULL) {
        printf("Error opening the file!\n");
        return;
    }
    User user;
    printf("Enter your username: ");
    scanf("%19s", user.username); // Ensure username is no more than 19 characters
    printf("Enter the password: ");
    scanf("%19s", user.password); // Ensure password is no more than 19 characters
    printf("Enter the CNP: ");
    scanf("%13s", user.cnp); // Ensure CNP is no more than 13 characters
    if (strlen(user.cnp) != 13) {
        printf("The CNP entered is not valid. Length must be 13 characters.\n");
        fclose(file);
        return;
    }
    fprintf(file, "%s %s %s\n", user.username, user.password, user.cnp);
    fclose(file);
    printf("User added successfully!\n");

    User* newNode = (User*)malloc(sizeof(User));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newNode->username, user.username);
    strcpy(newNode->password, user.password);
    strcpy(newNode->cnp, user.cnp);
    newNode->headAccount = NULL;
    newNode->tailAccount = NULL;
    newNode->urm = NULL;

    if (*head == NULL) {
        *head = newNode;
    }
    else {
        User* temp = *head;
        while (temp->urm != NULL) {
            temp = temp->urm;
        }
        temp->urm = newNode;
    }
}


void addIban(User* head) {
    char iban[35], cnp[14], currencyStr[4];
    float sold;
    Currency currency;

    printf("Enter the CNP of the user: ");
    scanf("%13s", cnp);
    User* user = SearchUserByCNP(head, cnp);
    if (!user) {
        printf("User with CNP %s not found.\n", cnp);
        return;
    }

    printf("Enter the IBAN code: ");
    scanf("%34s", iban); 
    if (SearchAccount(user->headAccount, iban)) {
        printf("The account with IBAN %s already exists.\n", iban);
        return;
    }

    printf("Enter the currency (RON, EUR, GBP): ");
    scanf("%3s", currencyStr); 
    currency = stringToCurrency(currencyStr);

    printf("Enter the initial balance: ");
    scanf("%f", &sold);

    account* newAccount = (account*)malloc(sizeof(account));
    if (!newAccount) {
        printf("Memory allocation failed!\n");
        return;
    }
    strcpy(newAccount->iban, iban);
    newAccount->currency = currency;
    newAccount->sold = sold;
    newAccount->urm = NULL;

    user = AddToEndAccount(user, newAccount);
    printf("IBAN added successfully!\n");
}


void deleteIban(User* head) {
    char cnp[14], iban[24];

    printf("Enter the CNP of the user: ");
    scanf("%s", cnp);
    User* user = SearchUserByCNP(head, cnp);
    if (!user) {
        printf("User with CNP %s not found.\n", cnp);
        return;
    }

    printf("Enter the IBAN code to delete: ");
    scanf("%s", iban);

    account* prev = NULL, * curr = user->headAccount;
    while (curr && strcmp(curr->iban, iban) != 0) {
        prev = curr;
        curr = curr->urm;
    }

    if (!curr) {
        printf("The account with IBAN %s not found.\n", iban);
        return;
    }

    if (prev) {
        prev->urm = curr->urm;
    }
    else {
        user->headAccount = curr->urm;
    }

    if (curr == user->tailAccount) {
        user->tailAccount = prev;
    }

    free(curr);
    printf("IBAN deleted successfully!\n");
}

void editUser(User* head) {
    char cnp[14];
    printf("Enter the CNP of the user to edit: ");
    scanf("%s", cnp);
    User* user = SearchUserByCNP(head, cnp);
    if (!user) {
        printf("User with CNP %s not found.\n", cnp);
        return;
    }

    printf("Editing user %s\n", user->username);
    printf("Enter new username: ");
    scanf("%s", user->username);
    printf("Enter new password: ");
    scanf("%s", user->password);
    printf("User edited successfully!\n");
}

void changePassword(User* head, User* loggedInUser) {
    if (loggedInUser == NULL) {
        printf("Error: No user is logged in.\n");
        return;
    }

    char newPassword[20];
    printf("Enter new password: ");
    scanf("%19s", newPassword); 
    strcpy(loggedInUser->password, newPassword);
    printf("Password changed successfully!\n");
}


void transferMoney(User* head, User* loggedInUser) {
    char sourceIban[24], targetIban[24];
    float amount;

    printf("Enter source IBAN: ");
    scanf("%23s", sourceIban);
    account* sourceAccount = SearchAccount(loggedInUser->headAccount, sourceIban);
    if (!sourceAccount) {
        printf("Source IBAN not found.\n");
        return;
    }

    printf("Enter target IBAN: ");
    scanf("%s", targetIban);
    User* user = head;
    account* targetAccount = NULL;
    while (user && !targetAccount) {
        targetAccount = SearchAccount(user->headAccount, targetIban);
        user = user->urm;
    }

    if (!targetAccount) {
        printf("Target IBAN not found.\n");
        return;
    }

    printf("Enter amount to transfer: ");
    scanf("%f", &amount);
    if (amount > sourceAccount->sold) {
        printf("Insufficient funds.\n");
        return;
    }

    sourceAccount->sold -= amount;
    targetAccount->sold += amount;
    printf("Transfer successful!\n");
}

void payUtilityBills(User* loggedInUser) {
    char iban[24];
    float amount;

    printf("Enter IBAN to pay from: ");
    scanf("%23s", iban); 
    account* acc = SearchAccount(loggedInUser->headAccount, iban);
    if (!acc) {
        printf("IBAN not found.\n");
        return;
    }

    printf("Enter amount to pay: ");
    scanf("%f", &amount);
    if (amount > acc->sold) {
        printf("Insufficient funds.\n");
        return;
    }

    acc->sold -= amount;
    printf("Utility bill paid successfully!\n");
}


void otherOperations(User* loggedInUser) {
    printf("Other operations are currently not implemented.\n");
}

void adminLogin(User* head) {
    char adminUsername[20], adminPassword[20];

    printf("Enter admin username: ");
    scanf("%s", adminUsername);
    printf("Enter admin password: ");
    scanf("%s", adminPassword);

    if (strcmp(adminUsername, "admin") == 0 && strcmp(adminPassword, "admin123") == 0) {
        int adminChoice;
        do {
            AdminMenu();
            printf("Enter your choice: ");
            scanf("%d", &adminChoice);

            switch (adminChoice) {
            case 1: addUser(&head); break;
            case 2: addIban(head); break;
            case 3: ShowUsers(head); break;
            case 4: deleteIban(head); break;
            case 5: editUser(head); break;
            case 6: changePassword(head, NULL); break; 
            default: printf("Invalid choice. Try again.\n");
            }
        } while (adminChoice != 7);
    }
    else {
        printf("Invalid admin credentials!\n");
    }
}

void userLogin(User* head) {
    char username[20], password[20];

    printf("Enter username: ");
    scanf("%19s", username); // Ensure username input is limited
    printf("Enter password: ");
    scanf("%19s", password); // Ensure password input is limited

    User* user = SearchUserByUsername(head, username);
    if (user && strcmp(user->password, password) == 0) {
        int userChoice;
        User* loggedInUser = user; // Ensure loggedInUser is set correctly

        do {
            UserMenu();
            printf("Enter your choice: ");
            scanf("%d", &userChoice);

            switch (userChoice) {
            case 1: ShowAccounts(loggedInUser->headAccount); break;
            case 2: ShowAccounts(loggedInUser->headAccount); break; // Display extracts
            case 3: transferMoney(head, loggedInUser); break;
            case 4: payUtilityBills(loggedInUser); break;
            case 5: otherOperations(loggedInUser); break;
            case 6: changePassword(head, loggedInUser); break;
            case 7: printf("Signing off...\n"); break;
            default: printf("Invalid choice. Try again.\n");
            }
        } while (userChoice != 7);
    }
    else {
        printf("Invalid user credentials!\n");
    }
}


int main() {
    User* users = ReadUsersFromFile("users.txt");
    ReadAccountsFromFile(users, "accounts.txt");

    int mainChoice;
    do {
        printf("\n---Main Menu---\n");
        printf("1. Admin login\n");
        printf("2. User login\n");
        printf("3. Exit\n");
        printf("Enter the option: ");
        scanf("%d", &mainChoice);

        switch (mainChoice) {
        case 1: adminLogin(users); break;
        case 2: userLogin(users); break;
        case 3: printf("Exiting the program.\n"); break;
        default: printf("Invalid choice. Try again.\n");
        }
    } while (mainChoice != 3);

    return 0;
}
