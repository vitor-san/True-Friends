#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

struct user_{
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
	
};

void add_name(User *u, char *name){
	strcpy(u->name, name);
}
void add_age(User *u, int age){
	u->age = age;
}
void add_gender(User *u, char *gender){
	strcpy(u->gender, gender);
}
void add_currenty_city(User *u, char *current_city){
	strcpy(u->current_city, current_city);
}
void add_origin_city(User *u, char *origin_city){
	strcpy(u->origin_city, origin_city);
}
void add_footbal_club(User *u, char *footbal_club){
	strcpy(u->footbal_club, footbal_club);
}
void add_type_musical(User *u, char *type_musical){
	strcpy(u->gender, type_musical);
}
void add_type_movie(User *u, char *type_movie){
	strcpy(u->type_movie, type_movie);
}
void add_favorite_food(User *u, char *favorite_food){
	strcpy(u->favorite_food,favorite_food);
}
void add_interest(User *u, char *interest){
	strcpy(u->interest, interest);
}
User *new_user(){
	User *n = (User*) malloc(sizeof(User));
	return n;
}

void remove_user(User *u){
	free(u);
}



