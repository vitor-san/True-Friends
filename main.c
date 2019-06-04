#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void addProfile(Graph network) {
        
}

void addFriend() {}
void findFriend() {}
void findMatch() {}
void listProfile() {}
void myProfile() {}

void printLogo() {

    printf(""
"                                                  ..\n"
"                                                  :o+/-\n"
"                                                  :ooooo/.\n"
"                                                  :ooooooo+.\n"
"                                                 `+ooooooooo//.\n"
"                                      `-         /++oooooooooo:\n"
"                                    -+o+.      `/++++++++oooooo/`\n"
"                                  .+o+++/    `:++++++++++++++ooo/\n"
"                                 -+++++++:-:+++++++++++++++++++++:\n"
"                                :+++++++++++++++++++++++++++++++++`\n"
"                               .++++++++++++++++++++++++++++++++++:\n"
"                               :++++++++++++++++++++++++++++++++++/\n"
"                               :++++++++++++++++++++++++++++++++++/\n"
"                               -++++++++++++++++++++++++++++++++++:\n"
"                               `++++++++++++++++++++++++++++++++++`\n"
"                                -++++++++++++++++++++++++++++++++.\n"
"                                 -++++++++++++++++++++++++++++++.\n"
"                                  ./++++++++++///+/+++++++++++/`\n"
"                                    ./++++++/////////+++++++:.\n"
"                                      `-:/+////////////+/:.`\n"
"                                          `..---::---..`\n"
);

}


void printMenu() {

    printf("---------------------\n");
    printf("Choose an option (type the number)\n");
    printf("[1] add a new profile\n");
    printf("[2] add a friend\n");
    printf("[3] find a posible friend\n");
    printf("[4] find the perfect match\n");
    printf("[5] list all the profiles\n");
    printf("[6] see my profile\n");
    printf("[0] Exit\n");
    printf("---------------------\n");

}

int main(int argc, char const *argv[]) {

    int op = -1;
    Graph network = newGraph(100,0,freeNetwork());

    printLogo();

    while(op != 0) {
        printMenu();
        scanf("%d", &op);

        switch(op) {
            case 1 :
                addProfile(network);
                break;
            case 2 :
                addFriend();
                break;
            case 3 :
                findFriend();
                break;
            case 4 :
                findMatch();
                break;
            case 5 :
                listProfile();
                break;
            case 6 :
                myProfile();
                break;
            default :
                printf("Not an option");
                break;
        }

        system("clear");

    }

    return 0;
}
