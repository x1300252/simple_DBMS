#include <stdio.h>
#include <string.h>
#include "WhereState.h"
#include "SelectState.h"
#include "JoinState.h"
#include "Command.h"
#include "Table.h"

void join_state_handler(Table_t *table, Command_t *cmd, int arg_idx) {
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
    } else if (!strncmp(cmd->args[arg_idx], "id2", 3)) {
        cmd->cmd_args.sel_args.is_join = 2;
    } else {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    arg_idx++;
    if (arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "where", 5)) {
            where_state_handler(table, cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "offset", 6)) {
            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "limit", 5)) {
            limit_state_handler(cmd, arg_idx+1);
            return;
        }   
    }
}

void count_join(Table_t *table, Command_t *cmd) {
    size_t join_len = 0; 
    size_t idx;
    extern std::unordered_map<unsigned int, int> index_id1;
    extern std::unordered_map<unsigned int, int> index_id2;
    std::unordered_map<unsigned int, int>::const_iterator got;

    if (cmd->cmd_args.sel_args.is_join == 1) { // join id1
        if (cmd->select_cols.idxListLen == -1) { // no where
            for (idx = 0; idx < table->len; idx++) {
                got = index_id1.find(table->users[idx].id);
                if (got != index_id1.end())
                    join_len += got->second;
            }
        } else { // where
            for (idx = 0; idx < cmd->select_cols.idxListLen; idx++) {
                got = index_id1.find(table->users[cmd->select_cols.idxList[idx]].id);
                if (got != index_id1.end())
                    join_len += got->second;
            }
        }
    } else  {
        if (cmd->select_cols.idxListLen == -1) { // no where
            for (idx = 0; idx < table->len; idx++) {
                got = index_id2.find(table->users[idx].id);
                if (got != index_id2.end())
                    join_len += got->second;
            }
        } else { // where
            for (idx = 0; idx < cmd->select_cols.idxListLen; idx++) {
                got = index_id2.find(table->users[cmd->select_cols.idxList[idx]].id);
                if (got != index_id2.end())
                    join_len += got->second;
            }
        }
    }
    
    cmd->select_cols.idxListLen = join_len;
}
