#include <stdio.h>
#include <string.h>
#include "WhereState.h"
#include "SelectState.h"
#include "JoinState.h"
#include "Command.h"
#include "Table.h"

void join_state_handler(Command_t *cmd, int arg_idx) {
    if ((arg_idx+4) > cmd->args_len) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    if (strncmp(cmd->args[arg_idx], "like", 4)) {
        cmd->type = UNRECOG_CMD;
        return;
    } else if (strncmp(cmd->args[arg_idx+1], "on", 2)) {
        cmd->type = UNRECOG_CMD;
        return;
    } else if (strncmp(cmd->args[arg_idx+2], "id", 2)) {
        cmd->type = UNRECOG_CMD;
        return;
    } else if (strncmp(cmd->args[arg_idx+3], "=", 1)) {
        cmd->type = UNRECOG_CMD;
        return;
    }

    arg_idx += 4;
    if (!strncmp(cmd->args[arg_idx], "id1", 3)) {
        cmd->cmd_args.sel_args.is_join = 1;
        return;
    } else if (!strncmp(cmd->args[arg_idx], "id2", 3)) {
        cmd->cmd_args.sel_args.is_join = 2;
        return;
    } else {
        cmd->type = UNRECOG_CMD;
        return;
    }
}

void count_join(Table_t *table, Command_t *cmd) {
    
}
