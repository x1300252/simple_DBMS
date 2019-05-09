#ifndef UPDATE_STATE_H
#define UPDATE_STATE_H

#include "WhereState.h"

void update_state_handler(Table_t *table, Command_t *cmd);
void update_id_handler(Table_t *table, SelectCols_t selectCols, int new_id);
void update_name_handler(Table_t *table, SelectCols_t selectCols, const char* new_name);
void update_email_handler(Table_t *table, SelectCols_t selectCols, const char* new_email);
void update_age_handler(Table_t *table, SelectCols_t selectCols, int new_age);

#endif