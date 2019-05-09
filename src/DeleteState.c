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
    selectCols.idxListLen = 0;
    
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
    
    // if (cmd->args_len > 3
    //         && !strncmp(cmd->args[arg_idx], "where", 5)) {
        
    // }
    
    if (selectCols.idxListLen) {
        int idx;
        for (idx=0; idx<selectCols.idxListLen; idx++) {
            delete_col(table, selectCols.idxList[idx]);
        }
    }
    else {
        delete_all(table);
    }
}

void delete_col(Table_t *table, int idx) {
    memcpy(table->users+idx, table->users+idx+1, (table->len-idx-1)*sizeof(User_t));
    memset(table->users+table->len-1, 0, sizeof(User_t));
    table->len--;
}

void delete_all(Table_t *table) {
    memset(table->users, 0, sizeof(User_t)*table->len);
    table->len = 0;
}
