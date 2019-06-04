#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
<<<<<<< HEAD
#include "user.h"
=======

typedef struct user_{
	char name[50];
	int age;
	char gender[5];
	char current_city[50];
	char origin_city[50];
	char footbal_club[50];
	char type_musical[50];
	char type_movie[50];
	char favorite_food[50];
	char interest[50];

}User;
>>>>>>> 87ac34be1029fbbb129e2a53d045772f0958fcc1

int count_peoples(FILE *fp){
	int cnt=0, flag=1;
	char c;
	while((c = fgetc(fp)) != EOF){
		if(cnt%10 == 0 && flag){flag=0; continue;}
		if(c == '\n'){cnt++; flag =1;}
	}
	return cnt/10;
}

void read_item(FILE *fp, User **list_users, int pos, int type){
	char c;
	char input[50];
	int i=0;
	fread(&c, sizeof(char), 1, fp);
	while(c != '\n'){
		input[i]=c;
		i++;
		fread(&c, sizeof(char), 1, fp);
	}
	input[i] ='\0';

	switch(type){
		case 1:
			add_name(list_users[pos], input);
			break;
		case 2:
			add_gender(list_users[pos], input);
			break;
		case 3:
			add_currenty_city(list_users[pos], input);
			break;
		case 4:
			add_origin_city(list_users[pos], input);
			break;
		case 5:
			add_footbal_club(list_users[pos], input);
			break;
		case 6:
			add_type_musical(list_users[pos], input);
			break;
		case 7:
			add_type_movie(list_users[pos], input);
			break;
		case 8:
			add_favorite_food(list_users[pos], input);
			break;
		case 9:
<<<<<<< HEAD
			add_interest(list_users[pos], input);		
			break;		
=======
			strcpy(list_users[pos].interest, input);
			break;
>>>>>>> 87ac34be1029fbbb129e2a53d045772f0958fcc1
	}

}

void read_users(FILE *fp, User **list_users, int number_users){
	int i=0, age;
	char c, aux[20];
	for(int i=0; i<number_users; i++){
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, list_users, i, 1);
		fseek(fp, 7, SEEK_CUR);
		fscanf(fp, "%d", &age);
		add_age(list_users[i], age);	
		fseek(fp, 7, SEEK_CUR);
		read_item(fp, list_users, i, 2);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, list_users, i, 3);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, list_users, i, 4);
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, list_users, i, 5);
		fseek(fp, 16, SEEK_CUR);
		read_item(fp, list_users, i, 6);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, list_users, i, 7);
		fseek(fp, 17, SEEK_CUR);
		read_item(fp, list_users, i, 8);
		fseek(fp, 11, SEEK_CUR);
		read_item(fp, list_users, i, 9);
		fseek(fp, 1, SEEK_CUR);
	}

void addProfile(Graph network) {

    static int id = 0;
    User* new;
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

    if(!searchVertex(network,compareName,meU)) {
        printf("The user does not exist");
        return;
    }

    printf("What is the name of the person that you're searching\n");
    scanf("%[\n\r]",other);
    otherU->name = other;

    if(!searchVertex(network,compareName,otherU)) {
        printf("The user does not exist");
        return;
    }

    //TODO: colocar a medida do peso
    other = strcat(other,".txt");
    //that file contains the name of the person
    //who sent a friend invite to the file name person
    FILE* fp = fopen(other,"r+");
    //write the name of the person who sent the invite
    fprintf(fp,"%s\n",me);

    fclose(fp);

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

	FILE *fp = fopen("pessoas.txt", "r");
	int number_users = count_peoples(fp);
<<<<<<< HEAD
	User **list_users = (User**) malloc(sizeof(User*)*number_users);
	for(int i=0; i<number_users; i++) list_users[i] = new_user():
=======
	rewind(fp);
	User *list_users = (User*) malloc(sizeof(User)*(number_users+1));
>>>>>>> 87ac34be1029fbbb129e2a53d045772f0958fcc1
	read_users(fp, list_users, number_users);
	
    int op = -1;
    Graph network = newGraph(100,0,removeUser());

    printLogo();
    while(op != 0) {
        printMenu();
        scanf("%d", &op);

        switch(op) {
            case 0 :
                continue;
            case 1 :
                addProfile(network);
                break;
            case 2 :
                addFriend(network);
                break;
            case 3 :
                findFriend(network);
                break;
            case 4 :
                findMatch(network);
                break;
            case 5 :
                listProfile(network);
                break;
            case 6 :
                myProfile(network);
                break;
            default :
                printf("Not an option");
                break;
        }
        system("clear");
    }
<<<<<<< HEAD
    
    for(int i=0; i<number_users; i++) remove_user(list_users[i]);
    free(list_users);
	fclose(fp);	
=======

	free(list_users);
	fclose(fp);
>>>>>>> 87ac34be1029fbbb129e2a53d045772f0958fcc1
    return 0;
}
