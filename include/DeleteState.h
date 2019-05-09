#ifndef DELETE_STATE_H
#define DELETE_STATE_H

#include "WhereState.h"

void delete_state_handler(Table_t *table, Command_t *cmd);
void delete_col(Table_t *table, int idx);
void delete_all(Table_t *table);

#endif