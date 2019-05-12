#ifndef WHERE_STATE_H
#define WHERE_STATE_H

#include "Command.h"
#include "Table.h"

int search_operator(Command_t *cmd, char *arg);
void where_state_handler(Table_t *table, Command_t *cmd, int arg_idx);
int (*get_int_ope(const char* ope))(int a, int b);
int (*get_str_ope(const char* ope))(const char* a, const char* b);
int (*get_bit_ope(const char* ope))(int a, int b);
int int_eq(int a, int b);
int int_neq(int a, int b);
int int_gt(int a, int b);
int int_gte(int a, int b);
int int_lt(int a, int b);
int int_lte(int a, int b);
int str_eq(const char* a, const char* b);
int str_neq(const char* a, const char* b);
int bit_and(int a, int b);
int bit_or(int a, int b);

#endif
