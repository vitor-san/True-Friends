#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "user.h"

#define FRIEND_THRESHOLD 0.45
#define MATCH_THRESHOLD 0.65

User *loggedIn;
int myId;
/*
 * This function return the number of people in the file
 */ 
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
/*
 * Auxiliary function of read_users
 */ 
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
/*
 * This function read all features the users and store in the array 
 */ 
void read_users(FILE *fp, User **users_list, int number_users) {
	int i = 0, age;
	for(int i = 0; i < number_users; i++){
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, users_list, i, 1);
		fseek(fp, 5, SEEK_CUR);
		fscanf(fp, "%d", &age);
		setAge(users_list[i], age);
		fseek(fp, 9, SEEK_CUR);
		read_item(fp, users_list, i, 2);
		fseek(fp, 14, SEEK_CUR);
		read_item(fp, users_list, i, 3);
		fseek(fp, 13, SEEK_CUR);
		read_item(fp, users_list, i, 4);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, users_list, i, 5);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, users_list, i, 6);
		fseek(fp, 13, SEEK_CUR);
		read_item(fp, users_list, i, 7);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, users_list, i, 8);
		fseek(fp, 15, SEEK_CUR);
		read_item(fp, users_list, i, 9);
		fseek(fp, 1, SEEK_CUR);
	}
}

void inputError() {
	fprintf(stderr, "\n\tIncorrect input! Finishing session...\n");
	exit(1);
}

/*
	Returns the string passed as parameter, but without whitespaces.
	This function DO NOT modify the original string.
*/
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

/*
	Create a user file (under the directory "Profiles"),
	which will hold the user's friends and friendship requests.
*/
void createUserFile(User *u) {
	char *noSpace = noWhitespace(getName(u));
	char path[100] = "./Profiles/";
	strcat(noSpace, ".txt");
	strcat(path, noSpace);
	free(noSpace);

 	FILE *fp = fopen(path, "w");
	fclose(fp);
}

/*
	Open a user file under "Profile" directory,
	and return it's address in memory.
*/
FILE *openUserFile(User *u,char *mode) {
	char *noSpace = noWhitespace(getName(u));
	char path[100] = "./Profiles/";
	strcat(noSpace, ".txt");
	strcat(path, noSpace);
	free(noSpace);

 	return fopen(path, mode);
}

/*
	Add a user to the Network graph.
*/
int addProfile(Graph network, User *u) {
    static int id = 0;
    addVertex(network, id);
    setVertexData(network, id, u);
	id++;
	return id-1;	//the added profile id
}

/*
	Generic function for user removal.
*/
void removeProfile(void *user) {
	removeUser((User *)user);
}

/*
	Generic function for user printing.
*/
void printProfile(void *user) {
	printUser((User *)user);
}

/*
	Return the similarity between two users
	age (value is between 0 and 1).
*/
double ageSimilarity(int age1, int age2) {
	if (age1 < age2) return age1/(double)age2;
	return age2/(double)age1;
}

/*
	Return the similarity between two users,
	considering a friendship relation
	(value is between 0 and 1).
*/
double friendSimilarity(User *a, User *b) {
	double const weight[] = {0.25, 0.2, 0.1, 0.1, 0.2, 0.1, 0.05};
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

/*
	Return the similarity between two users,
	considering a match between them
	(value is between 0 and 1).
*/
double matchSimilarity(User *a, User *b) {
	double const weight[] = {0.3, 0.3, 0.05, 0.05, 0.15, 0.05, 0.1};
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

/*
	Build all the edges (friendships) of our network.
*/
void buildNetwork(Graph network) {
	for (int i = 0; i < numVertices(network); i++) {
		User *cur = getVertexData(network, i);
		FILE *userFile = openUserFile(cur,"r+");

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

					double sim = friendSimilarity(cur, friend);
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
	}
}
/*
 * This function show all friendship requests
 */ 
int showMyFriendRequests(FILE *fp, Graph network, int showIndex) {
	printf("\n\tFriendship requests:\n\t");
	int count = 0;

	if (!feof(fp)) {	//this profile has friendship requests
		fgetc(fp);	//jumps '\n'
		int k = 0;
		char name[50];

		char c = fgetc(fp);
		while (c != '$' && !feof(fp)) {
			if (c == '\n') {
				name[k] = '\0';
				User *friend = searchVertexReturnData(network, compareName, name);
				double sim = friendSimilarity(loggedIn, friend);
				if (showIndex) printf("[%d] ", ++count);
				printf("%s ", name);
				if (sim < FRIEND_THRESHOLD) printf("- might not be a true friendship\n\t");
				else printf("- probably a true friendship!\n\t");
				k = 0;
			}
			else name[k++] = c;
			c = fgetc(fp);
		}
	}
	else printf("NONE\n\t");

	return count;
}

void showMyProfile(Graph network) {
	printf("\n\t=====================================================\n");
	printUser(loggedIn);
	printf("\n\t");
	FILE *fp = openUserFile(loggedIn,"r+");

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
				double sim = friendSimilarity(loggedIn, friend);
				printf("%s ", name);
				if (sim < FRIEND_THRESHOLD) printf("- might not be a true friendship\n\t");
				else printf("\n\t");
				k = 0;
			}
			else name[k++] = c;
			c = fgetc(fp);
		}
	}
	else printf("NONE\n\t");
	showMyFriendRequests(fp, network, 0);
	printf("=====================================================");

	fclose(fp);
}

int hasSendFriendRequest(char *yourName, User *target) {
	FILE *fp = openUserFile(target, "r");
	char c = fgetc(fp);
	if (c == '#') while (c != '$' && !feof(fp)) c = fgetc(fp);	//jump friends section

	if (c == '$') {	//this profile has friendship requests
		fgetc(fp);	//jumps '\n'
		int k = 0;
		char name[50];

		c = fgetc(fp);
		while (c != '$' && !feof(fp)) {
			if (c == '\n') {
				name[k] = '\0';
				if (strcmp(name, yourName) == 0) {
					fclose(fp);
					return 1;
				}
				k = 0;
			}
			else name[k++] = c;
			c = fgetc(fp);
		}
	}

	fclose(fp);
	return 0;
}

void addFriend(Graph network) {
	char search[51];
    printf("\n\tWhat's the name of the person that you want to add?\n\t)> ");
    fgets(search, 51, stdin);
	if (search[strlen(search)-1] == '\n') search[strlen(search)-1] = '\0';

	User *found = searchVertexReturnData(network, compareName, search);
    if (found == NULL) {
        printf("\n\tThe user does not exist.");
        return;
    }

	int idFound = searchVertexReturnPos(network, compareName, search);
	if (isAdjacent(network, myId, idFound)) {
		printf("\n\tYou're already friend of this person.");
		return;
	}

	if (hasSendFriendRequest(getName(loggedIn), found)) {	//voce ja mandou a solicitacao de amizade para aquela pessoa
		printf("\n\tYou've already sent a friendship request to this person.");
		return;
	}

	if (hasSendFriendRequest(getName(found), loggedIn)) {
		printf("\n\tThe person you want to add has already sent a friendship request to you.\n\t");
		return;
	}

	if (friendSimilarity(loggedIn, found) < FRIEND_THRESHOLD) {
		printf("\n\tAre you sure? This person might not be a true friend... (Y/N)\n\t)> ");
		char opt;
		scanf("%c", &opt);
		getchar();	//throws '\n' away
		if (opt == 'N') return;
		else if (opt == 'Y') {
			FILE *fp = openUserFile(found,"r+");	//this file contains the name of the person who sent a friend invite to the file name person
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
			printf("\n\tFriendship request sent!");
		}
		else inputError();
	}
	else {
		FILE *fp = openUserFile(found,"r+");	//this file contains the name of the person who sent a friend invite to the file name person
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
		printf("\n\tFriendship request sent!");
	}
}

/*
	Update the user's file after a friend request was accepted.
*/
void updateFile(Graph network, char *user_change, char *name_accept){

	User *user_ = searchVertexReturnData(network, compareName, user_change);
	FILE *fp = openUserFile(user_ , "r");
	FILE *aux  = fopen("auxiliary.txt", "w+");
	char name[51], tag_1 = '#', tag_2 = '$', skip_line = '\n', c;
	int count = 0;

	//writes in the file auxiliary.txt the updated file
	fread(&c, sizeof(char), 1, fp);
	fwrite(&tag_1, sizeof(char), 1, aux);
	fwrite(&skip_line, sizeof(char), 1, aux);
	if(c == tag_1){
		c = fgetc(fp);
		while(c != tag_2 && !feof(fp)){
			fscanf(fp, "%[^\n]", name);
			fwrite(name, strlen(name), 1,aux);
			fwrite(&skip_line, sizeof(char), 1, aux);
			c = fgetc(fp);
			c = fgetc(fp);
			if(c != tag_2 && !feof(fp)) fseek(fp, -1, SEEK_CUR);
		}
	}
	fwrite(name_accept, strlen(name_accept), 1, aux);
	fwrite(&skip_line, sizeof(char), 1, aux);
	int first_writing=1;
	if(c == tag_2){
		c = fgetc(fp);
		while(!feof(fp)){
			fscanf(fp, "%[^\n]", name);
			if(strcmp(name, name_accept) != 0){
				if(first_writing){
					fwrite(&tag_2, sizeof(char), 1, aux);
					fwrite(&skip_line, sizeof(char), 1, aux);
					first_writing = 0;
				}
				fwrite(name, strlen(name), 1, aux);
				fwrite(&skip_line, sizeof(char),1, aux);
			}
			c = fgetc(fp);
			c = fgetc(fp);
			if(!feof(fp)) fseek(fp, -1, SEEK_CUR);
		}
	}
	fclose(fp);
	rewind(aux);
	fp = openUserFile(user_,"w+");//in this file all content was deleted
	c = fgetc(aux);
	//writing the new content in the origin file
	if(c == tag_1){
		fwrite(&c, sizeof(char), 1, fp);
		fwrite(&skip_line, sizeof(char), 1, fp);
		c = fgetc(aux);
		while(c != tag_2 && !feof(aux)){
			fscanf(aux, "%[^\n]", name);
			fwrite(name, strlen(name), 1, fp);
			fwrite(&skip_line, sizeof(char), 1, fp);
			c = fgetc(aux);
			c = fgetc(aux);
			if(!feof(aux) && c != tag_2) fseek(aux, -1, SEEK_CUR);
		}
	}
	if(c == tag_2){
		fwrite(&c, sizeof(char), 1, fp);
		fwrite(&skip_line, sizeof(char), 1, fp);
		c = fgetc(aux);
		while(!feof(aux)){
			fscanf(aux, "%[^\n]", name);
			fwrite(name, strlen(name), 1, fp);
			fwrite(&skip_line, sizeof(char), 1, fp);
			c = fgetc(aux);
			c = fgetc(aux);
			if(!feof(aux)) fseek(aux, -1, SEEK_CUR);
		}
	}
	fclose(aux);
	fclose(fp);
	remove("auxiliary.txt");
}
/*
 * This function accepts some of the friendship requests,
 * if not exist returns to menu.
 */ 
void acceptFriend(Graph network) {

	FILE *fp = openUserFile(loggedIn,"r+");	//this file contains the name of people who sent a friendship invite to the logged in person

	char c = fgetc(fp);
	if (c == '#') while (c != '$' && !feof(fp)) c = fgetc(fp);	//jump friends section
	if (feof(fp)) {
		printf("\n\tYou don't have friendship requests.\n\t");
		return;
	}

	int number_requests = showMyFriendRequests(fp, network, 1);
	int number_accept;
	printf("\n\tWhat's the number of the person that you want to accept?\n\t)> ");
	scanf("%d", &number_accept);
	while(number_accept <= 0 || number_accept > number_requests){
		printf("\n\tInvalid number. Please, type again:\n\t)> ");
		scanf("%d", &number_accept);
	}
	rewind(fp);
	char name_accept[51];
	int count_aux=1;
	c = fgetc(fp);
	if (c == '#') while (c != '$' && !feof(fp)) c = fgetc(fp);	//jump friends section
	if(c == '$'){ //geting the name of the person that will be accepted
		while(1){
			c = fgetc(fp);
			fscanf(fp, "%[^\n]", name_accept);
			if(count_aux == number_accept)break;
			count_aux++;
			strcpy(name_accept, "");
		}
	}
	printf("\n\t%s was accepted.", name_accept);
	//union two new friends in the graph
	User *new_friend = searchVertexReturnData(network, compareName, name_accept);
	int pos = searchVertexReturnPos(network, compareName, name_accept);
	double sim = friendSimilarity(loggedIn, new_friend);
	if (addEdge(network, myId, pos)) {
		setEdgeCost(network, myId, pos, sim);
	}
	fclose(fp);
	//updating the two files
	updateFile(network, getName(loggedIn), name_accept);
	updateFile(network, name_accept, getName(loggedIn));
}

void removeFriendFromFile(User* userFile, Graph network,char target[51]) {
	FILE *fp = openUserFile(userFile,"r+");	//this file contains the name of the person who sent a friend invite to the file name person

	char c = fgetc(fp);
	c = fgetc(fp);
	char friends[numVertices(network)][51],friendsInvite[numVertices(network)][51];
	char tempLine[51];
	int pos = 0,posI = 0;

	while(tempLine[0] != '$' && !feof(fp)) {
		fscanf(fp,"%[^\n]\n",tempLine);
		if(strcmp(tempLine,target)) {
			strcpy(friends[pos],tempLine);
			pos++;
		}
	}

	while(!feof(fp)) {
		fscanf(fp,"%[^\n]\n",tempLine);
		if(strcmp(tempLine,target)) {
			strcpy(friendsInvite[posI],tempLine);
			posI++;
		}
	}

	fclose(fp);
	fp = openUserFile(userFile,"w+");	//this file contains the name of the person who sent a friend invite to the file name person

	if (pos > 1 || friends[0][0] != '$') {
		fprintf(fp,"#\n");
	}

	for (int i = 0; i < pos; i++) {
		fprintf(fp,"%s\n",friends[i]);
	}

	for (int i = 0; i < posI; i++) {
		fprintf(fp,"%s\n",friendsInvite[i]);
	}

	fclose(fp);

}


void removeFriend(Graph network) {

	char search[51];
    printf("\n\tWhat's the name of the person that you want to remove?\n\t)> ");
    fgets(search, 51, stdin);
	if (search[strlen(search)-1] == '\n') search[strlen(search)-1] = '\0';

	int found = searchVertexReturnPos(network, compareName, search);
	User* foundUser = searchVertexReturnData(network, compareName, search);

    if (found == -1) {
        printf("\n\tThe user does not exist.");
        return;
    }

	if (!removeEdge(network,found,myId)) {	//if cannot remove edge
		//you don't even have this person as your friend
		printf("\n\tYou are not friends with this person.");
        return;
	}

	removeFriendFromFile(loggedIn,network,search);
	removeFriendFromFile(foundUser,network,getName(loggedIn));

	printf("\n\tFriend removed.");

}

/*
	Find, amongst all users that are not friends
	with the logged in user, the one with most
	friend similarity with him.
*/
void findFriend(Graph network) {
	User *possFriend = NULL;
	double baseSim = FRIEND_THRESHOLD;

	for (int i = 0; i < numVertices(network); i++) {
		if (!isAdjacent(network, myId, i) && i != myId) {
			User *cur = getVertexData(network, i);
			double sim = friendSimilarity(loggedIn, cur);
			if (sim > baseSim) {
				baseSim = sim;
				possFriend = cur;
			}
		}
	}

	if (possFriend == NULL) {
		printf("\n\tOur system has not identified a friend with high enough similarity to you.");
		return;
	}
	printf("\n\tYou would probably like to start a friendship with:\n\t");
	printf("%s", getName(possFriend));
}

/*
	Find, amongst all users, the one with most
	match similarity with the logged in user
	(considering his interest).
*/
void findMatch(Graph network) {
	User *match = NULL;
	double baseSim = MATCH_THRESHOLD;
	char *myInterest = getInterest(loggedIn);

	if (strcmp(myInterest, "None") == 0) {
		printf("\n\tYou have no interest in either men or women.");
		return;
	}

	if (strcmp(myInterest, "Men") == 0) {
		for (int i = 0; i < numVertices(network); i++) {
			if (i == myId) continue;

			User *cur = getVertexData(network, i);
			char *personGender = getGender(cur);

			if (strcmp(personGender, "Male") == 0) {
				double sim = matchSimilarity(loggedIn, cur);
				if (sim > baseSim) {
					baseSim = sim;
					match = cur;
				}
			}
		}
	}
	else if (strcmp(myInterest, "Women") == 0) {
		for (int i = 0; i < numVertices(network); i++) {
			if (i == myId) continue;

			User *cur = getVertexData(network, i);
			char *personGender = getGender(cur);

			if (strcmp(personGender, "Female") == 0) {
				double sim = matchSimilarity(loggedIn, cur);
				if (sim > baseSim) {
					baseSim = sim;
					match = cur;
				}
			}
		}
	}
	else {	//you are interested in both
		for (int i = 0; i < numVertices(network); i++) {
			if (i == myId) continue;

			User *cur = getVertexData(network, i);
			double sim = matchSimilarity(loggedIn, getVertexData(network, i));
			if (sim > baseSim) {
				baseSim = sim;
				match = getVertexData(network, i);
			}
		}
	}

	if (match == NULL) {
		printf("\n\tOur current base does not have a perfect match for you.");
		return;
	}
	printf("\n\tYour perfect match is with:\n\t");
	printf("%s", getName(match));
}

/*
	List all current profiles in the network
	(without listing the loggen in user himself).
*/
void listProfiles(Graph network) {
	printf("\n\t==================================================");
	for (int i = 0; i < numVertices(network); i++) {
		User *cur = getVertexData(network, i);
		if (cur == loggedIn) continue;
		else printf("\n");
		printProfile(cur);
		printf("\t==================================================");
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

/*
	Setup to create a new profile and add it
	to the network.
*/
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
	printf("[1] Male\t[2] Female\n\t======> ");
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
	printf("[1] Men    [2] Women    [3] Both    [4] None\n\t======> ");
	scanf("%d", &intBuffer);
	getchar();	//gets '\n' from stdin
	if (intBuffer == 1) setInterest(new, "Men");
	else if (intBuffer == 2) setInterest(new, "Women");
	else if (intBuffer == 3) setInterest(new, "Both");
	else if (intBuffer == 4) setInterest(new, "None");
	else inputError();

	return new;
}

/*
	Program's first screen. Check if the person
	using it already have an account or not, and
	in case not, let him create one.
*/
void welcomeUser(Graph network, FILE *fp) {
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
			freopen(NULL, "a", fp);	//reopens file for append operations
			loggedIn = newProfile();
			myId = addProfile(network, loggedIn);
			createUserFile(loggedIn);
			fputc('\n', fp);
			printUserToFile(loggedIn, fp);
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
	printf("\t[3] accept a friendship request\n\t");
	printf("\t[4] remove a friend\n\t");
	printf("\t[5] find a possible friend\n\t");
	printf("\t[6] find the perfect match\n\t");
    printf("\t[7] list all profiles\n\t");
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
	welcomeUser(network, fp);

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
				acceptFriend(network);
				break;
			case 4:
				removeFriend(network);
				break;
            case 5:
                findFriend(network);
                break;
            case 6:
                findMatch(network);
                break;
            case 7:
                listProfiles(network);
                break;
            default:
                printf("\n\tInvalid option!\n");
        }
    }

    for (int i = 0; i < number_users; i++) removeUser(users_list[i]);
    free(users_list);
	fclose(fp);
	finishSession();
    return 0;
}
