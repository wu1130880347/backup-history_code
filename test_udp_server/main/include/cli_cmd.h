#ifndef __CLI_CMD__
#define __CLI_CMD__

#include "semphr.h"
#include "stdint.h"

#define SET_EXAMPLE 0
#define SET_OK      1
#define SET_STATE   2
#define SET_FIALED  3

extern uint8_t cli_get_flag;
extern uint8_t cmd_line[40];
extern uint8_t str_init_bps[10];

extern uint8_t cli_init(void);
extern uint8_t cli_help_log(uint8_t log_flag);
#endif