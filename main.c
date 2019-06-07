#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "user.h"

#define THRESHOLD 0.6

int count_people(FILE *fp) {
	int cnt = 0, flag = 1;
	char c;
	while ((c = fgetc(fp)) != EOF) {
		if (cnt%10 == 0 && flag) {
			flag = 0;
			continue;
		}
		if (c == '\n') {
			cnt++;
			flag = 1;
		}
	}
	return cnt/10;
}

void read_item(FILE *fp, User **users_list, int pos, int type) {
	char c;
	char input[50];
	int i = 0;
	fread(&c, sizeof(char), 1, fp);
	while(c != '\n') {
		input[i++] = c;
		fread(&c, sizeof(char), 1, fp);
	}
	input[i] = '\0';

	switch(type) {
		case 1:
			setName(users_list[pos], input);
			break;
		case 2:
			setGender(users_list[pos], input);
			break;
		case 3:
			setCurrentCity(users_list[pos], input);
			break;
		case 4:
			setOriginCity(users_list[pos], input);
			break;
		case 5:
			setFootballClub(users_list[pos], input);
			break;
		case 6:
			setMusicalGenre(users_list[pos], input);
			break;
		case 7:
			setMovieGenre(users_list[pos], input);
			break;
		case 8:
			setFavoriteFood(users_list[pos], input);
			break;
		case 9:
			setInterest(users_list[pos], input);
			break;
	}
}

void read_users(FILE *fp, User **users_list, int number_users) {
	int i = 0, age;
	for(int i = 0; i < number_users; i++){
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, users_list, i, 1);
		fseek(fp, 7, SEEK_CUR);
		fscanf(fp, "%d", &age);
		setAge(users_list[i], age);
		fseek(fp, 7, SEEK_CUR);
		read_item(fp, users_list, i, 2);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, users_list, i, 3);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, users_list, i, 4);
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, users_list, i, 5);
		fseek(fp, 16, SEEK_CUR);
		read_item(fp, users_list, i, 6);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, users_list, i, 7);
		fseek(fp, 17, SEEK_CUR);
		read_item(fp, users_list, i, 8);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, users_list, i, 9);
		fseek(fp, 1, SEEK_CUR);
	}
}

void inputError() {
	fprintf(stderr, "Incorrect input! Finishing session...\n");
	exit(1);
}

//This function DO NOT modify the original string
char *noWhitespace(char *string) {
	char *returnString = calloc(strlen(string), sizeof(char));
	char copy[51];
	strcpy(copy, string);

	int i = 0, j = 0;
	while (copy[i] != '\0') {
		if (copy[i] != ' ') {
			returnString[j++] = copy[i];
		}
		i++;
	}
	return returnString;
}

FILE *openUserFile(User *u) {
	char *noSpace = noWhitespace(getName(u));
	char path[100] = "./Profiles/";
	strcat(noSpace, ".txt");
	strcat(path, noSpace);
	free(noSpace);

 	return fopen(path, "r+");
}

void addProfile(Graph network, User *u) {
    static int id = 0;
    addVertex(network, id);
    setVertexData(network, id, u);
	id++;
}

void removeProfile(void *user) {
	removeUser((User *)user);
}

void printProfile(void *user) {
	printUser((User *)user);
}

double ageSimilarity(int age1, int age2) {
	if (age1 < age2) return age1/(double)age2;
	return age2/(double)age1;
}

double similarity(User *a, User *b) {
	double const weight[] = {0.2, 0.2, 0.1, 0.1, 0.2, 0.1, 0.1};
	double sim = 0;

	sim += weight[0]*ageSimilarity(getAge(a), getAge(b));
	if (strcmp(getCurrentCity(a), getCurrentCity(b)) == 0) sim += weight[1];
	if (strcmp(getOriginCity(a), getOriginCity(b)) == 0) sim += weight[2];
	if (strcmp(getFootballClub(a), getFootballClub(b)) == 0) sim += weight[3];
	if (strcmp(getMusicalGenre(a), getMusicalGenre(b)) == 0) sim += weight[4];
	if (strcmp(getMovieGenre(a), getMovieGenre(b)) == 0) sim += weight[5];
	if (strcmp(getFavoriteFood(a), getFavoriteFood(b)) == 0) sim += weight[6];

	return sim;
}

//This function will build all the edges (friendships) of our network.
void buildNetwork(Graph network) {
	for (int i = 0; i < numVertices(network); i++) {
		User *cur = getVertexData(network, i);
		FILE *userFile = openUserFile(cur);

		//printf("Current user: %s\n", getName(cur));
		if (userFile == NULL) {
			fprintf(stderr, "Error loading dataset\n");
			exit(1);
		}

		char c = fgetc(userFile);
		if (c == '#') {	//this profile has friends
			fgetc(userFile);	//ignores the next '\n'
			int k = 0;
			char name[50];

			c = fgetc(userFile);
			while (c != '$' && !feof(userFile)) {
				if (c == '\n') {
					name[k] = '\0';
					User *friend = searchVertexReturnData(network, compareName, name);
					int pos = searchVertexReturnPos(network, compareName, name);

					double sim = similarity(cur, friend);
					if (addEdge(network, i, pos)) {
						setEdgeCost(network, i, pos, sim);
					}
					k = 0;
				}
				else name[k++] = c;
				c = fgetc(userFile);
			}
		}

		fclose(userFile);
		//printGraph(network, printProfile, 0);
	}
}



void addFriend(Graph network) {
	char me[50], other[50];
    User *myU, *otherU;

    printf("What is your name?\n");
    scanf("%[\n\r]",me);
    setName(myU,me);

	User *found = searchVertexReturnData(network,compareName,myU);
    if (found == NULL) {
        printf("The user does not exist");
        return;
    }
	memcpy(myU, found, getUserSize(myU));

    printf("What is the name of the person that you're searching?\n");
    scanf("%[\n\r]", other);
    setName(otherU, other);

	found = searchVertexReturnData(network,compareName,otherU);
    if (found == NULL) {
        printf("The user does not exist");
        return;
    }
	memcpy(otherU, found, getUserSize(otherU));

	if (similarity(myU, otherU) < THRESHOLD) {
		printf("Are you sure? This person might not be a true friend... (Y/N)\n");
		char opt;
		scanf("%c", &opt);
		if (opt == 'N') return;
		else {
			strcat(other,".txt");
		    //that file contains the name of the person
		    //who sent a friend invite to the file name person
		    FILE* fp = fopen(other,"a");
		    //write the name of the person who sent the invite
		    fprintf(fp,"%s\n",me);

		    fclose(fp);
		}
	}
	else {
		strcat(other,".txt");
	    //that file contains the name of the person
	    //who sent a friend invite to the file name person
	    FILE* fp = fopen(other,"a");
	    //write the name of the person who sent the invite
	    fprintf(fp,"%s\n",me);

	    fclose(fp);
	}
}

void findFriend(Graph network) {

	char me[50];
	User *meU,*iterator;

	printf("What is your name?\n");
    scanf("%[\n\r]",me);
	setName(meU,me);

	if(!searchVertexReturnData(network,compareName,meU)) {
        printf("The user does not exist");
        return;
    }

	int numUsers = numVertices(network);

	for (int i = 0; i < numUsers; i++) {
		//iterator = network->list[i];
		//TODO: calcular a semelhanca
	}



}

void findMatch(Graph network) {

}

void listProfile(Graph network) {
	//TODO: just use the function printGraph and make the function of printing
}

void myProfile(Graph network) {

	char me[50];
	User *meU;

	printf("What is your name?\n");
    scanf("%[\n\r]", me);
	setName(meU, me);

	if(!searchVertexReturnData(network, compareName, meU)) {
        printf("The user does not exist");
        return;
    }

	//TODO: listar os invites de amizade
	//TODO: listar os amigos atuais

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

void finishSession() {
	printf("Finishing session...\n");
	exit(0);
}

void welcomeUser() {
	printf("\n\t=========================== Welcome to TrueFriends.com! ===========================\n");
	printf("\t\t\t\tDo you already have an account? (Y/N)\n\t");

	char ans;
	scanf("%c", &ans);

	if (ans == 'Y') {
		printf("\tWhat's your name?\n\t");
		char name[51];
		fgets(name, 51, stdin);
		if (name[strlen(name)-1] == '\n') name[strlen(name)-1] = '\0';
		puts(name);
	}
	else if (ans == 'N') {
		printf("\n\tDo you want to create one?\n\t");
		scanf("%c", &ans);

		if (ans == 'Y') {
			printf("\tYour profile has been sucessfully created!\n\t");

		}
		else if (ans == 'N') finishSession();
		else inputError();
	}
	else inputError();
}

void printMenu() {

    printf("\tChoose an option (type the number)\n\t");
	printf("\t[1] see my profile\n\t");
	printf("\t[2] add a friend\n\t");
	printf("\t[3] find a possible friend\n\t");
	printf("\t[4] find the perfect match\n\t");
    printf("\t[5] list all profiles\n\t");
    printf("\t[0] exit\n\n\t");

}

int main(int argc, char const *argv[]) {

	FILE *fp = fopen("profiles.txt", "r+");
	int number_users = count_people(fp);
	User **users_list = (User**) malloc(sizeof(User*)*number_users);
	for(int i = 0; i < number_users; i++) users_list[i] = newUser();
	rewind(fp);
	read_users(fp, users_list, number_users);

    int op = -1;
    Graph network = newGraph(1, 0, removeProfile);
	for (int i = 0; i < number_users; i++) addProfile(network, users_list[i]);
	buildNetwork(network);

	system("clear");
    printLogo();
	welcomeUser();

    while(op != 0) {
        printMenu();
        scanf("%d", &op);

        switch(op) {
            case 0 :
                continue;
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
                printf("\tNot an option\n");
                break;
        }
        system("clear");
    }

    for(int i = 0; i < number_users; i++) removeUser(users_list[i]);
    free(users_list);
	freeGraph(network);
	fclose(fp);

    return 0;
}
