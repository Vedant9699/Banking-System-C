#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_USERS 10
#define MIN_BALANCE 50.00 
typedef struct {
    int id;
    char name[50];
    char pin[5];
    double balance;
    bool isActive;
} User;

bool authenticate(User users[], int count, int *currentIdx);
void processTransaction(User *u, int type);
void clearBuffer();

int main() {
    User db[MAX_USERS] = {
        {101, "Alice Smith", "1234", 1500.75, true},
        {102, "Bob Jones", "4321", 50.00, true}
    };
    int userCount = 2;
    int loggedInIdx = -1;

    printf("--- 🏦 SECURE CORE BANKING SYSTEM v2.0 ---\n");

    if (!authenticate(db, userCount, &loggedInIdx)) {
        printf("\n[SECURITY ALERT]: Multiple failed attempts. Locking session.\n");
        return 1;
    }

    int choice;
    while (1) {
        printf("\nWelcome, %s | Balance: $%.2f", db[loggedInIdx].name, db[loggedInIdx].balance);
        printf("\n1. Deposit\n2. Withdraw\n3. Logout\nSelection: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please use numeric values.\n");
            clearBuffer();
            continue;
        }

        if (choice == 3) break;
        if (choice == 1 || choice == 2) {
            processTransaction(&db[loggedInIdx], choice);
        } else {
            printf("Operation not recognized.\n");
        }
    }

    printf("\nSession terminated safely. Thank you.\n");
    return 0;
}


bool authenticate(User users[], int count, int *currentIdx) {
    int inputID;
    char inputPIN[5];
    int attempts = 0;

    while (attempts < 3) {
        printf("\nEnter Account ID: ");
        if (scanf("%d", &inputID) != 1) {
            clearBuffer();
            printf("Numeric ID required.\n");
            attempts++;
            continue;
        }
        
        printf("Enter 4-Digit PIN: ");
        scanf("%4s", inputPIN);

        for (int i = 0; i < count; i++) {
            if (users[i].id == inputID && strcmp(users[i].pin, inputPIN) == 0) {
                *currentIdx = i;
                return true;
            }
        }
        attempts++;
        printf("Invalid ID or PIN. (%d/3 attempts used)\n", attempts);
    }
    return false;
}

void processTransaction(User *u, int type) {
    double amount;
    printf("Enter amount: $");
    if (scanf("%lf", &amount) != 1 || amount <= 0) {
        printf("[ERROR]: Transaction aborted. Invalid amount.\n");
        clearBuffer();
        return;
    }

    if (type == 2) { 
        if (u->balance - amount < MIN_BALANCE) {
            printf("[DENIED]: Transaction violates Minimum Balance ($%.2f).\n", MIN_BALANCE);
        } else {
            u->balance -= amount;
            printf("[SUCCESS]: Withdrawal complete.\n");
        }
    } else {
        u->balance += amount;
        printf("[SUCCESS]: Deposit processed.\n");
    }
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}