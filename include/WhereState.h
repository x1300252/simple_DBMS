#ifndef WHERE_STATE_H
#define WHERE_STATE_H

#include "Command.h"

typedef struct SelectCols {
    int *idxList;
    size_t idxListLen;
} SelectCols_t;


int search_operator(Command_t *cmd, char *arg);

#endif
