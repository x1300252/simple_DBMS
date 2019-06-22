#include <stdio.h>
#include <string.h>
#include "WhereState.h"
#include "SelectState.h"
#include "Command.h"
#include "Table.h"

const char *operators[6]= {">=", "<=", "!=", "=", ">", "<"};

int (*l_int_ope)(int, int);
int (*r_int_ope)(int, int);
int (*l_str_ope)(const char*, const char*);
int (*r_str_ope)(const char*, const char*);
int (*bit_ope)(int, int);

int (*int_ope_arr[])(int, int) = {int_gte, int_lte, int_neq, int_eq, int_gt, int_lt};
int (*str_ope_arr[])(const char*, const char*) = {str_neq, str_eq};

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

void where_state_handler(Table_t *table, Command_t *cmd, int arg_idx) {
    /*
        About where with id1, id2 filter:
        first we record the value of id1, id2 to join_id1_val, join_id2_val
     */
    int l_result = 0;
    int r_result = 0;
    int result;
    int l_val;
    int r_val;
    char * l_str;
    char * r_str;
    int idx;
    
    cmd->select_cols.idxListLen = 0;
    cmd->select_cols.idxList = (int*)malloc(sizeof(int) * (table->len*table->len_like));
    
    if ((arg_idx+2) > cmd->args_len) {
        cmd->type = UNRECOG_CMD;
        return;
    }

    l_int_ope = NULL;
    l_str_ope = NULL;
    bit_ope = NULL;
    if (!strncmp(cmd->args[arg_idx], "id", 2)
            || !strncmp(cmd->args[arg_idx], "age", 3)
            || !strncmp(cmd->args[arg_idx], "id1", 3)
            || !strncmp(cmd->args[arg_idx], "id2", 3)) {
        l_val = atoi(cmd->args[arg_idx+2]);
        l_int_ope = get_int_ope(cmd->args[arg_idx+1]);
    }
    else if (!strncmp(cmd->args[arg_idx], "name", 2)
            || !strncmp(cmd->args[arg_idx], "email", 3)) {
        l_str_ope = get_str_ope(cmd->args[arg_idx+1]);
        l_str = cmd->args[arg_idx+2];
    }

    if (!strncmp(cmd->args[arg_idx], "id1", 3)) {
        cmd->cmd_args.sel_args.join_id1_val = l_val;
    }
    else if (!strncmp(cmd->args[arg_idx], "id2", 3)) {
        cmd->cmd_args.sel_args.join_id2_val = l_val;
    }

    if (!l_int_ope && !l_str_ope) {
        cmd->type = UNRECOG_CMD;
        return;
    }
    
    // for(idx=0; idx<cmd->args_len; idx++) {
    //     printf("%s\n", cmd->args[idx]);
    // }
    
    if ((arg_idx+3) < cmd->args_len) {
        bit_ope = get_bit_ope(cmd->args[arg_idx+3]);
    }
    
    if (bit_ope == NULL) {
        if (!strcmp("id", cmd->args[arg_idx])) {
            for (idx = 0; idx < table->len; idx++) {
                result = l_int_ope(table->users[idx].id, l_val);
                if (result) {
                    cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                    cmd->select_cols.idxListLen++;
                }
            }
        }
        else if (!strcmp("name", cmd->args[arg_idx])) {
            for (idx = 0; idx < table->len; idx++) {
                result = l_str_ope(table->users[idx].name, l_str);
                if (result) {
                    cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                    cmd->select_cols.idxListLen++;
                }
            }
        }
        else if (!strcmp("email", cmd->args[arg_idx])) {
            for (idx = 0; idx < table->len; idx++) {
                result = l_str_ope(table->users[idx].email, l_str);
                if (result) {
                    cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                    cmd->select_cols.idxListLen++;
                }
            }
        }
        else if (!strcmp("age", cmd->args[arg_idx])) {
            for (idx = 0; idx < table->len; idx++) {
                result = l_int_ope(table->users[idx].age, l_val);
<<<<<<< HEAD
            } else if (!strcmp("id2", cmd->args[arg_idx])) {
                result = 1;
            } else if (!strcmp("id1", cmd->args[arg_idx])) {
                result = 1;
            }

            if (result) {
                row_matched_update(idx, result, cmd, table);
=======
                if (result) {
                    cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                    cmd->select_cols.idxListLen++;
                }
>>>>>>> yx
            }
        }
        arg_idx += 3;
    }
    else {
        int l_arg = arg_idx;
        int r_arg = arg_idx+4;

        r_int_ope = NULL;
        r_str_ope = NULL;
        
        if (!strncmp(cmd->args[r_arg], "id", 2)
                || !strncmp(cmd->args[r_arg], "age", 3)
                || !strncmp(cmd->args[r_arg], "id1", 3)
                || !strncmp(cmd->args[r_arg], "id2", 3)) {
            r_val = atoi(cmd->args[r_arg+2]);
            r_int_ope = get_int_ope(cmd->args[r_arg+1]);
        }

        if (!strncmp(cmd->args[r_arg], "id1", 3)) {
            cmd->cmd_args.sel_args.join_id1_val = l_val;
        }
        else if (!strncmp(cmd->args[r_arg], "id2", 3)) {
            cmd->cmd_args.sel_args.join_id2_val = l_val;
        }

        else if (!strncmp(cmd->args[r_arg], "name", 2)
                || !strncmp(cmd->args[r_arg], "email", 3)) {
            r_str_ope = get_str_ope(cmd->args[r_arg+1]);
            r_str = cmd->args[r_arg+2];
        }
        
        if (!r_int_ope && !r_str_ope) {
            cmd->type = UNRECOG_CMD;
            return;
        }
        
        if (!strcmp("id", cmd->args[l_arg])) {
            if (!strcmp("id", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].id, l_val);
                    r_result = r_int_ope(table->users[idx].id, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("name", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].id, l_val);
                    r_result = r_str_ope(table->users[idx].name, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("email", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].id, l_val);
                    r_result = r_str_ope(table->users[idx].email, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("age", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].id, l_val);
                    r_result = r_int_ope(table->users[idx].age, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
        }
        else if (!strcmp("name", cmd->args[l_arg])) {
            if (!strcmp("id", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].name, l_str);
                    r_result = r_int_ope(table->users[idx].id, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("name", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].name, l_str);
                    r_result = r_str_ope(table->users[idx].name, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("email", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].name, l_str);
                    r_result = r_str_ope(table->users[idx].email, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
<<<<<<< HEAD
            else if (!strcmp("age", cmd->args[l_arg])) {
                l_result = l_int_ope(table->users[idx].age, l_val);
            } else if (!strcmp("id2", cmd->args[l_arg])) {
                l_result = 1;
            } else if (!strcmp("id1", cmd->args[l_arg])) {
                l_result = 1;
=======
            else if (!strcmp("age", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].name, l_str);
                    r_result = r_int_ope(table->users[idx].age, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
>>>>>>> yx
            }
        }
        else if (!strcmp("email", cmd->args[l_arg])) {
            if (!strcmp("id", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].email, l_str);
                    r_result = r_int_ope(table->users[idx].id, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("name", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].email, l_str);
                    r_result = r_str_ope(table->users[idx].name, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("email", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].email, l_str);
                    r_result = r_str_ope(table->users[idx].email, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("age", cmd->args[r_arg])) {
<<<<<<< HEAD
                r_result = r_int_ope(table->users[idx].age, r_val);
            } else if (!strcmp("id2", cmd->args[r_arg])) {
                r_result = 1;
            } else if (!strcmp("id1", cmd->args[r_arg])) {
                r_result = 1;
            }
            
            result = bit_ope(l_result, r_result);
            //printf("%d %d %d %d\n", idx, l_result, r_result, result);
            if (result) {
                row_matched_update(idx, result, cmd, table);
=======
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_str_ope(table->users[idx].email, l_str);
                    r_result = r_int_ope(table->users[idx].age, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
        }
        else if (!strcmp("age", cmd->args[l_arg])) {
            if (!strcmp("id", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].age, l_val);
                    r_result = r_int_ope(table->users[idx].id, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("name", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].age, l_val);
                    r_result = r_str_ope(table->users[idx].name, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("email", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].age, l_val);
                    r_result = r_str_ope(table->users[idx].email, r_str);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
            }
            else if (!strcmp("age", cmd->args[r_arg])) {
                for (idx = 0; idx < table->len; idx++) {
                    l_result = l_int_ope(table->users[idx].age, l_val);
                    r_result = r_int_ope(table->users[idx].age, r_val);
                    result = bit_ope(l_result, r_result);
                    //printf("%d %d %d %d\n", idx, l_result, r_result, result);
                    if (result) {
                        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
                        cmd->select_cols.idxListLen++;
                    }
                }
>>>>>>> yx
            }
        }
        
        arg_idx += 7;
    }
    if (arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "offset", 6)) {
            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "limit", 5)) {
            limit_state_handler(cmd, arg_idx+1);
            return;
        }
        cmd->type = UNRECOG_CMD;
        return;
    }
    return;
}

int get_like_col(Like_t like,int col_type) {
    if (col_type == 1) {
        return like.id1;
    } else if (col_type == 2) {
        return like.id2;
    }
    return -1;
}

void find_match_table(Table_t *table, int col_type,  int (*l_int_ope)(int, int), int l_val, int (*r_int_ope)(int, int), int r_val , int (*bit_ope)(int, int)) {
    // TDOO: find if like has a match, if match add the count of matched likes, 
    uint64_t l_result, r_result, count;
    for (int i=0; i < table->len_like; i++) {
        if (l_int_ope) {
            l_result = l_int_ope(get_like_col(table->likes[i], col_type), l_val);
            if (l_result) {
                count++;
            }
        }
        if (r_int_ope && l_int_ope) {
            r_result = r_int_ope(get_like_col(table->likes[i], col_type), r_val);
            // bit 
            if (bit_ope(l_result, r_result)) {
                count++;
            }
        }
    }
    return count;
}

int is_joined(User_t  user, Like_t like, int join_field) {
    // check if user is joined with a like
    if (join_field == 1 && (user.id == like.id1)) {
        return 1;
    } else if (join_field == 2 && (user.id == like.id2)) {
        return 1;
    }
    return 0;
}

int check_join_value(User_t user, Table_t *table, int check_field, int check_value, int join_field) {
    int count;
    // return the total count of matched value
    for (int i=0; i< table->len_like; i++) {
        if (is_joined(user, table->likes[i], join_field )) {
            if (check_field == 1) {

            } else if (check_field == 2) {

            }

        }
    }
}

void row_matched_update(int idx, int result, Command_t *cmd, Table_t *table) {

    if (cmd->cmd_args.sel_args.is_join != 0) {
        if (cmd->cmd_args.sel_args.join_id2_val == -1 && cmd->cmd_args.sel_args.join_id1_val == -1) {
            cmd->select_cols.idxListLen++; // add the amt of count in hash map
        } else {
            cmd->select_cols.idxListLen++; 
        }
    } else {
        cmd->select_cols.idxList[cmd->select_cols.idxListLen] = idx;
        cmd->select_cols.idxListLen++;
    }
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
    for (idx = 2; idx < 4; idx++) {
        if (!strcmp(ope, operators[idx])) {
            return str_ope_arr[idx-2];
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