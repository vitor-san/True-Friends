#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


int count_peoples(FILE *fp){
	int cnt=0, flag=1;	
	char c;
	while((c = fgetc(fp)) != EOF){
		if(cnt%10 == 0 && flag){flag=0; continue;}
		if(c == '\n'){cnt++; flag =1;}
	}
	return cnt/10;
}

void read_item(FILE *fp, User *list_users, int pos, int type){
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
			strcpy(list_users[pos].name, input);
			break;
		case 2:
			strcpy(list_users[pos].gender, input);
			break;
		case 3:
			strcpy(list_users[pos].current_city, input);
			break;
		case 4:
			strcpy(list_users[pos].origin_city, input);
			break;
		case 5:
			strcpy(list_users[pos].footbal_club, input);
			break;
		case 6:
			strcpy(list_users[pos].type_musical, input);
			break;
		case 7:
			strcpy(list_users[pos].type_movie, input);
			break;
		case 8:
			strcpy(list_users[pos].favorite_food, input);
			break;
		case 9:
			strcpy(list_users[pos].interest, input);		
			break;		
	}

}

void read_users(FILE *fp, User *list_users, int number_users){
	int i=0, al=0;
	char c, aux[20];
	for(int i=0; i<number_users; i++){
		fseek(fp, 6, SEEK_CUR);
		read_item(fp, list_users, i, 1);
		fseek(fp, 7, SEEK_CUR);
		fscanf(fp, "%d", &list_users[i].age);
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
}

int main(){
	
	FILE *fp = fopen("pessoas.txt", "r");
	int number_users = count_peoples(fp);	
	rewind(fp);
	User *list_users = (User*) malloc(sizeof(User)*(number_users+1)); 
	read_users(fp, list_users, number_users);	
	free(list_users);
	fclose(fp);	
	return 0;
}