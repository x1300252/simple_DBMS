#ifndef COMMAND_H
#define COMMAND_H

enum { 
    UNRECOG_CMD,
    BUILT_IN_CMD,
    QUERY_CMD,
};

enum {
    INSERT_CMD = 100,
    SELECT_CMD,
    UPDATE_CMD,
    DELETE_CMD,
};

typedef struct {
    char name[256];
    int len;
    unsigned char type;
} CMD_t;

extern CMD_t cmd_list[];

typedef struct SelectArgs {
    char **fields;
    char ** aggr_funcs;
    size_t fields_len;
    size_t funcs_len;
    int offset;
    int limit;
    int is_join; // 0 no join, 1 join on id1, 2 join on id2
    int table_flag; // 1 for user, 2 for like
} SelectArgs_t;

typedef struct SelectCols {
    int *idxList;
    size_t idxListLen;
} SelectCols_t;

typedef union {
    SelectArgs_t sel_args;
} CmdArg_t;

typedef struct Command {
    unsigned char type;
    char **args;
    size_t args_len;
    size_t args_cap;
    CmdArg_t cmd_args;
    SelectCols_t select_cols;
} Command_t;

Command_t* new_Command();
int add_Arg(Command_t *cmd, const char *arg);
int add_select_field(Command_t *cmd, const char *argument);
int add_aggr_funcs(Command_t *cmd, const char *argument);
void cleanup_Command(Command_t *cmd);

#endif
