#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "parser.h"
#include "exe.h"
void debug_command(const Command* command);

void debug_arg(const Argument* arg);

void debug_command_result(const CommandResult* res);
#endif