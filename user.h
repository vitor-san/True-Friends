#ifndef USER_H
#define USER_H

typedef struct user_ User;

User *newUser();
void removeUser(User *u);
void printUser(User *u);

void setName(User *u, char *name);
char *getName(User *u);

void setAge(User *u, int age);
int getAge(User *u);

void setGender(User *u, char *gender);
char *getGender(User *u);

void setCurrentCity(User *u, char *current_city);
char *getCurrentCity(User *u);

void setOriginCity(User *u, char *origin_city);
char *getOriginCity(User *u);

void setFootballClub(User *u, char *football_club);
char *getFootballClub(User *u);

void setMusicalGenre(User *u, char *musical_genre);
char *getMusicalGenre(User *u);

void setMovieGenre(User *u, char *movie_genre);
char *getMovieGenre(User *u);

void setFavoriteFood(User *u, char *favorite_food);
char *getFavoriteFood(User *u);

void setInterest(User *u, char *interest);
char *getInterest(User *u);

int compareName(void* a, void* b);
int getUserSize(User *u);

#endif
