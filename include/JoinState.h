#ifndef JOIN_STATE_H
#define JOIN_STATE_H

#include "Command.h"
#include "Table.h"

void join_state_handler(Command_t *cmd, int arg_idx);
void count_join(Table_t *table, Command_t *cmd);

#endif