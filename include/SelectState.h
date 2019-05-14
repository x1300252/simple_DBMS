#ifndef SELECT_STATE_H
#define SELECT_STATE_H

#include "Command.h"
#include "Table.h"

void field_state_handler(Table_t *table, Command_t *cmd, size_t arg_idx);
void table_state_handler(Table_t *table, Command_t *cmd, size_t arg_idx);
void offset_state_handler(Command_t *cmd, size_t arg_idx);
void limit_state_handler(Command_t *cmd, size_t arg_idx);
int aggr_func_count (Table_t *table, Command_t *cmd);
double aggr_func_avg (Table_t *table, Command_t *cmd, size_t idx);
int aggr_func_sum (Table_t *table, Command_t *cmd, size_t idx);

#endif
