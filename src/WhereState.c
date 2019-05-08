#include <stdio.h>
#include <string.h>
#include "WhereState.h"

char *operators[6]= {"=", "!=", ">", "<", ">=", "<="};

int check_operator(Command_t *cmd, char *arg) {
    char *ope_ptr, *token, *str;
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