#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkPassword(char s[20])
{
    char *password = "jeiks";
    /*
    password = (char*) malloc(6);
    password[0] = 'j';
    password[1] = 'e';
    password[2] = 'i';
    password[3] = 'k';
    password[4] = 's';
    password[5] = '\0';
    */
    if ( strcmp(password, s) == 0 )
        return 1;
    else
        return 0;
}

void startTheSystem()
{
    int n1, n2;
    printf("Welcome to the system!!\n"
           "Please write 2 integers: ");
    scanf("%d %d", &n1, &n2);
    printf("The multiplcation of %d by %d is %d\n", n1, n2, n1*n2);
    printf("That's all folks!\n");
}

void quitError()
{
    printf("Sorry! Unfortunately you do not know the password!!\nBye bye!\n");
    exit(1);
}

int main()
{
    char password[20];

    printf("Password: ");
    scanf("%s", password);

    if ( checkPassword(password) )
        startTheSystem();
    else
        quitError();
}

