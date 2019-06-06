#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

struct user_{
	char name[50];
	int age;
	char gender[10];
	char currentCity[50];
	char originCity[50];
	char footballClub[50];
	char musicalGenre[50];	//maybe a list
	char movieGenre[50];	//maybe a list
	char favoriteFood[50];
	char interest[10];
};

void setName(User *u, char *name){
	strcpy(u->name, name);
}
char *getName(User *u) {
	return u->name;
}

void setAge(User *u, int age){
	u->age = age;
}
int getAge(User *u) {
	return u->age;
}

void setGender(User *u, char *gender){
	strcpy(u->gender, gender);
}
char *getGender(User *u) {
	return u->gender;
}

void setCurrentCity(User *u, char *currentCity){
	strcpy(u->currentCity, currentCity);
}
char *getCurrentCity(User *u) {
	return u->currentCity;
}

void setOriginCity(User *u, char *originCity){
	strcpy(u->originCity, originCity);
}
char *getOriginCity(User *u) {
	return u->originCity;
}

void setFootballClub(User *u, char *footballClub){
	strcpy(u->footballClub, footballClub);
}
char *getFootballClub(User *u) {
	return u->footballClub;
}

void setMusicalGenre(User *u, char *musicalGenre){
	strcpy(u->musicalGenre, musicalGenre);
}
char *getMusicalGenre(User *u) {
	return u->musicalGenre;
}

void setMovieGenre(User *u, char *movieGenre){
	strcpy(u->movieGenre, movieGenre);
}
char *getMovieGenre(User *u) {
	return u->movieGenre;
}

void setFavoriteFood(User *u, char *favoriteFood){
	strcpy(u->favoriteFood, favoriteFood);
}
char *getFavoriteFood(User *u) {
	return u->favoriteFood;
}

void setInterest(User *u, char *interest){
	strcpy(u->interest, interest);
}
char *getInterest(User *u) {
	return u->interest;
}

User *newUser(){
	User *n = (User*) malloc(sizeof(User));
	return n;
}

void removeUser(User *u){
	free(u);
}

void printUser(User *u) {
	printf("User name: %s\n", u->name);
	printf("Age: %d\n", u->age);
	printf("Gender: %s\n", u->gender);
	printf("Current City: %s\n", u->currentCity);
	printf("Origin City: %s\n", u->originCity);
	printf("Football Club: %s\n", u->footballClub);
	printf("Musical Genre: %s\n", u->musicalGenre);
	printf("Movie Genre: %s\n", u->movieGenre);
	printf("Favorite Food: %s\n", u->favoriteFood);
	printf("Interested in: %s\n", u->interest);
}

int compareName(void* a, void* b) {
	User* au = (User*) a;
    User* bu = (User*) b;
    return strcmp(au->name,bu->name);
}

int getUserSize(User *u) {
	return sizeof(*u);
}
