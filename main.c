#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "user.h"

#define THRESHOLD 0.6

User *loggedIn;
int myId;

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
	fprintf(stderr, "\n\tIncorrect input! Finishing session...\n");
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

int addProfile(Graph network, User *u) {
    static int id = 0;
    addVertex(network, id);
    setVertexData(network, id, u);
	id++;
	return id-1;	//the added profile id
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

void showMyProfile(Graph network) {
	printf("\n");
	printUser(loggedIn);
	printf("\n\t");
	FILE *fp = openUserFile(loggedIn);

	printf("Friends:\n\t");
	char c = fgetc(fp);

	if (c == '#') {	//this profile has friends
		fgetc(fp);	//jumps '\n'
		int k = 0;
		char name[50];

		c = fgetc(fp);
		while (c != '$' && !feof(fp)) {
			if (c == '\n') {
				name[k] = '\0';
				User *friend = searchVertexReturnData(network, compareName, name);
				double sim = similarity(loggedIn, friend);
				printf("%s ", name);
				if (sim < THRESHOLD) printf("(might not be a true friendship)\n\t");
				else printf("\n\t");
				k = 0;
			}
			else name[k++] = c;
			c = fgetc(fp);
		}
		printf("\n\t");
	}
	else printf("NONE\n\n\t");

	printf("Friendship requests:\n\t");

	if (c == '$') {	//this profile has friendship requests
		fgetc(fp);	//jumps '\n'
		int k = 0;
		char name[50];

		c = fgetc(fp);
		while (c != '$' && !feof(fp)) {
			if (c == '\n') {
				name[k] = '\0';
				User *friend = searchVertexReturnData(network, compareName, name);
				double sim = similarity(loggedIn, friend);
				printf("%s ", name);
				if (sim < THRESHOLD) printf("- might not be a true friendship\n\t");
				else printf("- probably a true friendship!\n");
				k = 0;
			}
			else name[k++] = c;
			c = fgetc(fp);
		}
	}
	else printf("NONE\n");

	fclose(fp);
}

//QUASE FINALIZADA: FALTA FAZER A VERIFICACAO DE QUE, SE VOCE JA MANDOU SOLICITACAO PARA A PESSOA (OU E AMIGO DELA), NAO PODE MANDAR OUTRA
void addFriend(Graph network) {
	char search[51];
    printf("\n\tWhat's the name of the person that you're searching?\n\t)> ");
    fgets(search, 51, stdin);
	if (search[strlen(search)-1] == '\n') search[strlen(search)-1] = '\0';

	User *found = searchVertexReturnData(network, compareName, search);
    if (found == NULL) {
        printf("\n\tThe user does not exist.\n");
        return;
    }

	int idFound = searchVertexReturnPos(network, compareName, search);
	if (isAdjacent(network, myId, idFound)) {
		printf("\n\tYou're already friend of this person.\n");
		return;
	}

	if (getEdgeCost(network, myId, idFound) < THRESHOLD) {
		printf("\n\tAre you sure? This person might not be a true friend... (Y/N)\n\t)> ");
		char opt;
		scanf("%c", &opt);
		getchar();	//throws '\n' away
		if (opt == 'N') return;
		else if (opt == 'Y') {
			FILE *fp = openUserFile(found);	//this file contains the name of the person who sent a friend invite to the file name person
			char c = fgetc(fp);
			if (c == '#') {
				while (c != '$' && !feof(fp)) c = fgetc(fp);
				if (feof(fp)) {
					fputc('$', fp);
					fputc('\n', fp);
				}
			}
			else if (feof(fp)) {
				fputc('$', fp);
				fputc('\n', fp);
			}

			freopen(NULL, "a", fp);
			fprintf(fp, "%s\n", getName(loggedIn));	//write the name of the person who sent the invite
		    fclose(fp);
			printf("\n\tFriendship request send!");
		}
		else inputError();
	}
	else {
		FILE *fp = openUserFile(found);	//this file contains the name of the person who sent a friend invite to the file name person
		char c = fgetc(fp);
		if (c == '#') {
			while (c != '$' && !feof(fp)) c = fgetc(fp);
			if (feof(fp)) {
				fputc('$', fp);
				fputc('\n', fp);
			}
		}
		else if (feof(fp)) {
			fputc('$', fp);
			fputc('\n', fp);
		}

		freopen(NULL, "a", fp);
		fprintf(fp, "%s\n", getName(loggedIn));	//write the name of the person who sent the invite
		fclose(fp);
		printf("\n\tFriendship request send!");
	}
}

void removeFriend(Graph network) {
	//TODO
}

void findFriend(Graph network) {
	//TODO
}

void findMatch(Graph network) {
	//TODO
}

void listProfiles(Graph network) {
	printf("\n");
	for (int i = 0; i < numVertices(network); i++) {
		User *cur = getVertexData(network, i);
		printProfile(cur);
		printf("\n");
	}
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
	printf("\n\tFinishing session...\n");
	exit(0);
}

User *newProfile() {
	User *new = newUser();
	char strBuffer[51];
	int intBuffer;

	printf("\n\tOk. First, type in your name: \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setName(new, strBuffer);

	printf("\n\tGreat! Now, your age: \n\t======> ");
	scanf("%d", &intBuffer);
	getchar();	//gets '\n' from stdin
	setAge(new, intBuffer);

	printf("\n\tChoose your gender\n\t");
	printf("[1] Male\t[2] Female\n\t");
	scanf("%d", &intBuffer);
	getchar();	//gets '\n' from stdin
	if (intBuffer == 1) setGender(new, "Male");
	else if (intBuffer == 2) setGender(new, "Female");
	else inputError();

	printf("\n\tWhat is the city you're currently living? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setCurrentCity(new, strBuffer);

	printf("\n\tWhat is the city you were born/raised? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setOriginCity(new, strBuffer);

	printf("\n\tWhat is your favorite football club? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setFootballClub(new, strBuffer);

	printf("\n\tWhat is your favorite musical genre? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setMusicalGenre(new, strBuffer);

	printf("\n\tWhat is your favorite movie genre? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setMovieGenre(new, strBuffer);

	printf("\n\tWhat is your favorite food? \n\t======> ");
	fgets(strBuffer, 51, stdin);
	if (strBuffer[strlen(strBuffer)-1] == '\n') strBuffer[strlen(strBuffer)-1] = '\0';
	setFavoriteFood(new, strBuffer);

	printf("\n\tChoose a relashionship interest\n\t");
	printf("[1] Men    [2] Women    [3] Both    [4] None\n\t");
	scanf("%d", &intBuffer);
	getchar();	//gets '\n' from stdin
	if (intBuffer == 1) setInterest(new, "Men");
	else if (intBuffer == 2) setInterest(new, "Women");
	else if (intBuffer == 3) setInterest(new, "Both");
	else if (intBuffer == 4) setInterest(new, "None");
	else inputError();

	return new;
}

void welcomeUser(Graph network) {
	printf("\n\t=========================== Welcome to TrueFriends.com! ===========================\n");
	printf("\t\t\t\tDo you already have an account? (Y/N)\n\t)> ");

	char ans;
	scanf("%c", &ans);
	getchar();	//ignores '\n'

	if (ans == 'Y') {
		printf("\n\tWhat's your name?\n\t)> ");
		char name[51];
		fgets(name, 51, stdin);
		if (name[strlen(name)-1] == '\n') name[strlen(name)-1] = '\0';

		loggedIn = searchVertexReturnData(network, compareName, name);
		if (loggedIn == NULL) inputError();
		myId = searchVertexReturnPos(network, compareName, name);
		printf("\n\tLogin completed!");
	}
	else if (ans == 'N') {
		printf("\n\tDo you want to create one? (Y/N)\n\t)> ");
		scanf("%c", &ans);
		getchar();	//ignores '\n'

		if (ans == 'Y') {
			loggedIn = newProfile();
			myId = addProfile(network, loggedIn);
			printUser(loggedIn);
			printf("\n\tYour profile has been sucessfully created!");
		}
		else if (ans == 'N') finishSession();
		else inputError();
	}
	else inputError();
}

void printMenu() {

	printf("\n\t_______________________________________\n\n");
    printf("\tChoose an option (type the number)\n\t");
	printf("\t[1] see my profile\n\t");
	printf("\t[2] add a friend\n\t");
	printf("\t[3] remove a friend\n\t");
	printf("\t[4] find a possible friend\n\t");
	printf("\t[5] find the perfect match\n\t");
    printf("\t[6] list all profiles\n\t");
    printf("\t[0] exit\n\t");
	printf(")> ");

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
	welcomeUser(network);

    while (op != 0) {
        printMenu();
        scanf("%d", &op);
		getchar();	//eats '\n'

        switch(op) {
            case 0:
                continue;
			case 1:
				showMyProfile(network);
				break;
            case 2:
                addFriend(network);
                break;
			case 3:
				removeFriend(network);
            case 4:
                findFriend(network);
                break;
            case 5:
                findMatch(network);
                break;
            case 6:
                listProfiles(network);
                break;
            default:
                printf("\tNot an option\n");
                break;
        }
    }

    for (int i = 0; i < number_users; i++) removeUser(users_list[i]);
    free(users_list);
	freeGraph(network);
	fclose(fp);

	finishSession();
    return 0;
}
