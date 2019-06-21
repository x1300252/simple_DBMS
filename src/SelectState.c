#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "Table.h"
#include "SelectState.h"
#include "WhereState.h"

void field_state_handler(Table_t *table, Command_t *cmd, size_t arg_idx) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.aggr_funcs = NULL;
    cmd->cmd_args.sel_args.funcs_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = -1;
    while(arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "*", 1)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "id2", 3)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "id1", 3)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "id", 2)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "name", 4)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "email", 5)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "age", 3)) {
            add_select_field(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "count", 5)) {
            add_aggr_funcs(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "avg", 3)) {
            add_aggr_funcs(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "sum", 3)) {
            add_aggr_funcs(cmd, cmd->args[arg_idx]);
        } else if (!strncmp(cmd->args[arg_idx], "from", 4)) {
            table_state_handler(table, cmd, arg_idx+1);
            return;
        }
         else {
            cmd->type = UNRECOG_CMD;
            return;
        }
        arg_idx += 1;
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void table_state_handler(Table_t *table, Command_t *cmd, size_t arg_idx) {
    size_t table_name_idx = arg_idx;
    if (arg_idx < cmd->args_len
            && (!strncmp(cmd->args[arg_idx], "user", 4)
                || !strncmp(cmd->args[arg_idx], "like", 4)
            )) {
        arg_idx++;
        if (arg_idx == cmd->args_len) {
            return;
        } else if (!strncmp(cmd->args[arg_idx], "where", 5)
            && !strncmp(cmd->args[table_name_idx], "user", 4)) {
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
    cmd->type = UNRECOG_CMD;
    return;
}

void offset_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        } else if (arg_idx < cmd->args_len
                && !strncmp(cmd->args[arg_idx], "limit", 5)) {

            limit_state_handler(cmd, arg_idx+1);
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void limit_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx]);

        arg_idx++;

        if (arg_idx == cmd->args_len) {
            return;
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

int aggr_func_count (Table_t *table, Command_t *cmd) {
    if (cmd->select_cols.idxListLen == -1) {
        return table->len;
    } else {
        return cmd->select_cols.idxListLen;
    }
}

double aggr_func_avg (Table_t *table, Command_t *cmd, size_t idx) {
    int cnt = aggr_func_count(table, cmd);
    int sum = aggr_func_sum(table, cmd, idx);
    
    if (sum == -1) {
        cmd->type = UNRECOG_CMD;
        return -1;
    }
    else if (sum && cnt) {
        return (double)sum/(double)cnt;
    }
    else {
        return 0;
    }
}

int aggr_func_sum (Table_t *table, Command_t *cmd, size_t idx) {
    size_t i;
    int sum = 0;
    if (!strncmp(cmd->cmd_args.sel_args.fields[idx], "id", 2)) {
        if (cmd->select_cols.idxListLen == -1) {
            for (i = 0; i < table->len; i++) {
                sum += get_User(table, i)->id;
            }
        } else {
            for (i = 0; i < cmd->select_cols.idxListLen; i++) {
                sum += get_User(table, cmd->select_cols.idxList[i])->id;
            }
        }
    }
    else if (!strncmp(cmd->cmd_args.sel_args.fields[idx], "age", 3)) {
       if (cmd->select_cols.idxListLen == -1) {
            for (i = 0; i < table->len; i++) {
                sum += get_User(table, i)->age;
            }
        } else {
            for (i = 0; i < cmd->select_cols.idxListLen; i++) {
                sum += get_User(table, cmd->select_cols.idxList[i])->age;
            }
        }
    }
    else {
        cmd->type = UNRECOG_CMD;
        return -1;
    }

    return sum;
}
