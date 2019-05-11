#include <stdio.h>
#include <string.h>
#include "WhereState.h"
#include "Command.h"
#include "Table.h"

char *operators[6]= {"=", "!=", ">", "<", ">=", "<="};


int (*l_int_ope)(int, int);
int (*r_int_ope)(int, int);
int (*l_str_ope)(const char*, const char*);
int (*r_str_ope)(const char*, const char*);
int (*bit_ope)(int, int);

int (*int_ope_arr[])(int, int) = {int_eq, int_neq, int_gt, int_lt, int_gte, int_lte};
int (*str_ope_arr[])(const char*, const char*) = {str_eq, str_neq};

int check_operator(Command_t *cmd, char *arg) {
    char *ope_ptr, *token;
    char *saveptr = NULL;
    int idx;

    for (idx=0; idx<6; idx++) {
        if ((ope_ptr = strstr(arg, operators[idx])) != NULL) {
            if (!strcmp(arg, operators[idx])) {
                return -1;
            }
            else if (ope_ptr != arg) {
                token = strtok_r(arg, operators[idx], &saveptr);
                add_Arg(cmd, token);
                add_Arg(cmd, operators[idx]);
                token = strtok_r(NULL, operators[idx], &saveptr);
                if (token)
                    add_Arg(cmd, token);
            }
            else {
                add_Arg(cmd, operators[idx]);
                token = strtok_r(arg, operators[idx], &saveptr);
                add_Arg(cmd, token);
            }
            return idx;
        }
    }
    return -1;
}

void where_state_handler(Table_t *table, Command_t *cmd, int arg_idx, SelectCols_t *select_cols) {
    int l_result;
    int r_result;
    int result;
    int l_val;
    int r_val;
    int idx;
    
    if ((arg_idx+2) > cmd->args_len) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    

    l_int_ope = NULL;
    l_str_ope = NULL;
    if (!strncmp(cmd->args[arg_idx], "id", 2)
            || !strncmp(cmd->args[arg_idx], "age", 3)) {
        l_val = atoi(cmd->args[arg_idx+2]);
        l_int_ope = get_int_ope(cmd->args[arg_idx+1]);
    }
    else if (!strncmp(cmd->args[arg_idx], "name", 2)
            || !strncmp(cmd->args[arg_idx], "email", 3)) {
        l_str_ope = get_str_ope(cmd->args[arg_idx+1]);
    }
    
    if (!l_int_ope && !l_str_ope) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    if ((arg_idx+3) == cmd->args_len) {
        for (idx = 0; idx < table->len; idx++) {
            if (!strcmp("id", cmd->args[arg_idx])) {
                result = l_int_ope(table->users[idx].id, l_val);
            }
            else if (!strcmp("name", cmd->args[arg_idx])) {
                result = l_str_ope(table->users[idx].name, cmd->args[arg_idx+2]);
            }
            else if (!strcmp("email", cmd->args[arg_idx])) {
                result = l_str_ope(table->users[idx].email, cmd->args[arg_idx+2]);
            }
            else if (!strcmp("age", cmd->args[arg_idx])) {
                result = l_int_ope(table->users[idx].age, l_val);
            }
            printf("%d %d\n", idx, result);
        }
        return;
    }
    
    int l_arg = arg_idx;
    arg_idx += 3;
    int r_arg = arg_idx+1;
    if ((arg_idx+4) == cmd->args_len){
        bit_ope = get_bit_ope(cmd->args[arg_idx]);
        
        if (bit_ope == NULL) {
            cmd->type = UNRECOG_CMD;
            return;
        }
        
        arg_idx++;
        r_int_ope = NULL;
        r_str_ope = NULL;
        if (!strncmp(cmd->args[arg_idx], "id", 2)
                || !strncmp(cmd->args[arg_idx], "age", 3)) {
            r_val = atoi(cmd->args[arg_idx+2]);
            r_int_ope = get_int_ope(cmd->args[arg_idx+1]);
        }
        else if (!strncmp(cmd->args[arg_idx], "name", 2)
                || !strncmp(cmd->args[arg_idx], "email", 3)) {
            r_str_ope = get_str_ope(cmd->args[arg_idx+1]);
        }
        
        if (!r_int_ope && !r_str_ope && !bit_ope) {
            cmd->type = UNRECOG_CMD;
            return;
        }
        
        for (idx = 0; idx < table->len; idx++) {
            if (!strcmp("id", cmd->args[l_arg])) {
                l_result = l_int_ope(table->users[idx].id, l_val);
            }
            else if (!strcmp("name", cmd->args[l_arg])) {
                l_result = l_str_ope(table->users[idx].name, cmd->args[l_arg+2]);
            }
            else if (!strcmp("email", cmd->args[l_arg])) {
                l_result = l_str_ope(table->users[idx].email, cmd->args[l_arg+2]);
            }
            else if (!strcmp("age", cmd->args[l_arg])) {
                l_result = l_int_ope(table->users[idx].age, l_val);
            }
            
            if (!strcmp("id", cmd->args[r_arg])) {
                r_result = r_int_ope(table->users[idx].id, r_val);
            }
            else if (!strcmp("name", cmd->args[r_arg])) {
                r_result = r_str_ope(table->users[idx].name, cmd->args[r_arg+2]);
            }
            else if (!strcmp("email", cmd->args[r_arg])) {
                r_result = r_str_ope(table->users[idx].email, cmd->args[r_arg+2]);
            }
            else if (!strcmp("age", cmd->args[r_arg])) {
                r_result = r_int_ope(table->users[idx].age, r_val);
            }
            
            result = bit_ope(l_result, r_result);
            printf("%d %d %d %d\n", idx, l_result, r_result, result);
        }
    }
    cmd->type = UNRECOG_CMD;
    return;
}

int (*get_int_ope(const char* ope))(int a, int b) {
    int idx;
    for (idx = 0; idx < 6; idx++) {
        if (!strcmp(ope, operators[idx])) {
            return int_ope_arr[idx];
        }
    }
    return NULL;
}

int (*get_str_ope(const char* ope))(const char* a, const char* b) {
    int idx;
    for (idx = 0; idx < 2; idx++) {
        if (!strcmp(ope, operators[idx])) {
            return str_ope_arr[idx];
        }
    }
    return NULL;
}

int (*get_bit_ope(const char* ope))(int a, int b) {
    if (!strcmp("and", ope)) {
        return bit_and;
    }
    if (!strcmp("or", ope)) {
        return bit_or;
    }
    return NULL;
}

int int_eq(int a, int b) {
    return a == b;
}

int int_neq(int a, int b) {
    return a != b;
}

int int_gt(int a, int b) {
    return a > b;
}

int int_gte(int a, int b) {
    return a >= b;
}

int int_lt(int a, int b) {
    return a < b;
}

int int_lte(int a, int b) {
    return a <= b;
}

int str_eq(const char* a, const char* b) {
    return !strcmp(a, b);
}

int str_neq(const char* a, const char* b) {
    return strcmp(a, b);
}

int bit_and(int a, int b) {
    return a && b;
}

int bit_or(int a, int b) {
    return a || b;
}