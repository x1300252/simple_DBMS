#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Table.h"
#include "User.h"
#include "Command.h"
#include "WhereState.h"
#include "UpdateState.h"

void update_state_handler(Table_t *table, Command_t *cmd) {
    cmd->select_cols.idxList = NULL;
    cmd->select_cols.idxListLen = -1;
    
    if (cmd->args_len < 6) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    if (strncmp(cmd->args[1], "user", 5)) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    if (strncmp(cmd->args[2], "set", 3)) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    if (strncmp(cmd->args[4], "=", 1)) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    if (cmd->args_len > 6
         && !strncmp(cmd->args[6], "where", 5)) {
        where_state_handler(table, cmd, 7);
    }
    
    if (!strncmp(cmd->args[3], "id", 2)) {
        update_id_handler(table, cmd->select_cols, atoi(cmd->args[5]));
    }
    else if (!strncmp(cmd->args[3], "name", 4)) {
        update_name_handler(table, cmd->select_cols, cmd->args[5]);
    }
    else if (!strncmp(cmd->args[3], "email", 5)) {
        update_email_handler(table, cmd->select_cols, cmd->args[5]);
    }
    else if (!strncmp(cmd->args[3], "age", 3)) {
        update_age_handler(table, cmd->select_cols, atoi(cmd->args[5]));
    }
    else {
        cmd->type = UNRECOG_CMD;
        return;
    }
}

void update_id_handler(Table_t *table, SelectCols_t select_cols, int new_id) {
    size_t idx;
    User_t *usr_ptr;
    
    // Check id doesn't exist in the table
    for (idx = 0; idx < table->len; idx++) {
        usr_ptr = get_User(table, idx);
        if (usr_ptr->id == new_id) {
            return;
        }
    }
    
    if (select_cols.idxListLen == -1 && table->len == 1) {
        usr_ptr = get_User(table, 0);
        usr_ptr->id = new_id;
    }
    else if (select_cols.idxListLen == 1) {
        usr_ptr = get_User(table, select_cols.idxList[0]);
        usr_ptr->id = new_id;
    }
}

void update_name_handler(Table_t *table, SelectCols_t select_cols, const char* new_name) {
    size_t idx;
    User_t *usr_ptr;
    
    if (select_cols.idxListLen == -1) {
        for (idx = 0; idx < table->len; idx++) {
            usr_ptr = get_User(table, idx);
            strncpy(usr_ptr->name, new_name, MAX_USER_NAME);
        }
    }
    else {
        for (idx = 0; idx < select_cols.idxListLen; idx++) {
            usr_ptr = get_User(table, select_cols.idxList[idx]);
            strncpy(usr_ptr->name, new_name, MAX_USER_NAME);
        }
    }
}

void update_email_handler(Table_t *table, SelectCols_t select_cols, const char* new_email) {
    size_t idx;
    User_t *usr_ptr;
    
    if (select_cols.idxListLen == -1) {
        for (idx = 0; idx < table->len; idx++) {
            usr_ptr = get_User(table, idx);
            strncpy(usr_ptr->email, new_email, MAX_USER_EMAIL);
        }
    }
    else {
        for (idx = 0; idx < select_cols.idxListLen; idx++) {
            usr_ptr = get_User(table, select_cols.idxList[idx]);
            strncpy(usr_ptr->email, new_email, MAX_USER_EMAIL);
        }
    }
}

void update_age_handler(Table_t *table, SelectCols_t select_cols, int new_age) {
    size_t idx;
    User_t *usr_ptr;
    
    if (select_cols.idxListLen == -1) {
        for (idx = 0; idx < table->len; idx++) {
            usr_ptr = get_User(table, idx);
            usr_ptr->age = new_age;
        }
    }
    else {
        for (idx = 0; idx < select_cols.idxListLen; idx++) {
            usr_ptr = get_User(table, select_cols.idxList[idx]);
            usr_ptr->age = new_age;
        }
    }
}