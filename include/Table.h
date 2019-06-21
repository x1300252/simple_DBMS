#ifndef TABLE_H
#define TABLE_H
#include <stdlib.h>
#include <stdio.h>
#include "User.h"
#include "Like.h"
#include <unordered_map>

#define INIT_TABLE_SIZE 10000
#define EXT_LEN 500

std::unordered_map<unsigned int, int> index_id1;
std::unordered_map<unsigned int, int> index_id2;
    
typedef struct Table {
    size_t capacity;
    size_t len;
    User_t *users;
    unsigned char *cache_map;
    FILE *fp;
    char *file_name;
    
    size_t capacity_like;
    size_t len_like;
    Like_t *likes;
    unsigned char *cache_map_like;
    
    int *ids;
} Table_t;

Table_t *new_Table(char *file_name);
int add_User(Table_t *table, User_t *user);
int add_Like(Table_t *table, Like_t *like);
int archive_table(Table_t *table);
int load_table(Table_t *table, char *file_name);
User_t* get_User(Table_t *table, size_t idx);
Like_t* get_Like(Table_t *table, size_t idx);

#endif
