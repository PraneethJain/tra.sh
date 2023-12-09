#ifndef __SIGNALS_H
#define __SIGNALS_H

#include "base.h"
int init_signals();
void SIGINT_handler(int sig);
void SIGTSTP_handler(int sig);
int init_signal(int sig, void (*func)(int));

#endif
