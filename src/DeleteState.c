#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"
#include "User.h"
#include "Command.h"
#include "WhereState.h"
#include "DeleteState.h"

void delete_state_handler(Table_t *table, Command_t *cmd) {
    SelectCols_t selectCols;
    selectCols.idxList = NULL;
    selectCols.idxListLen = -1;
    
    if (cmd->args_len < 3) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    if (strncmp(cmd->args[1], "from", 1)) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    if (strncmp(cmd->args[2], "table", 5)) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    if (cmd->args_len > 4
            && !strncmp(cmd->args[3], "where", 5)) {
        where_state_handler(table, cmd, 4, &selectCols);
    }

    if (selectCols.idxListLen > 0) {
        int idx;
        for (idx=selectCols.idxListLen-1; idx>=0; idx--) {
            delete_col(table, selectCols.idxList[idx]);
        }
    }
    else if (selectCols.idxListLen == -1) {
        delete_all(table);
    }
}

void delete_col(Table_t *table, int idx) {
    for (; idx < table->len-1; idx++) {
        table->users[idx] = table->users[idx+1];
    }
    memset(table->users+table->len-1, 0, sizeof(User_t));
    table->len--;
}

void delete_all(Table_t *table) {
    memset(table->users, 0, sizeof(User_t)*table->len);
    table->len = 0;
}
