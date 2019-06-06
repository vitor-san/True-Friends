#ifndef USER_H
#define USER_H

typedef struct user_ User;

User *new_user();
void remove_user(User *u);
void add_name(User *u, char *name);
void add_age(User *u, int age);
void add_gender(User *u, char *gender);
void add_currenty_city(User *u, char *current_city);
void add_origin_city(User *u, char *origin_city);
void add_footbal_club(User *u, char *footbal_club);
void add_type_musical(User *u, char *type_musical);
void add_type_movie(User *u, char *type_movie);
void add_favorite_food(User *u, char *favorite_food);
void add_interest(User *u, char *interest);
int compareName(void* a, void* b);
#endif
