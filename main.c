#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void addProfile(Graph network) {

    static int id = 0;
    User* new = readUser();
    addVertex(network,id);
    setVertexData(network,id,new);

    printf("Sucessfully created your profile\n");

}

int compareName(void* a, void* b) {

    User* au = (User*) a;
    User* bu = (User*) b;

    return strcmp(au->name,bu->name);

}


void addFriend(Graph network) {

    char me[50],other[50];
    User* meU,otherU;
    printf("what is your name\n");
    scanf("%[\n\r]",me);
    meU->name = me;

    if(!searchVertex(netork,compareName,meU)) {
        printf("The user does not exist");
        return;
    }

    printf("What is the name of the person that you're searching\n");
    scanf("%[\n\r]",other);
    otherU->name = other;

    if(!searchVertex(netork,compareName,otherU)) {
        printf("The user does not exist");
        return;
    }

    //TODO: colocar a medida do peso

}

void findFriend(Graph network) {

}

void findMatch(Graph network) {

}

void listProfile(Graph network) {

}

void myProfile(Graph network) {

}

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
