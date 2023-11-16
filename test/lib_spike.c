#include "lib.h"
#include <stdio.h>

void print_f(float x) { printf("%f", x); }

void print_c(char x) { printf("%c", x); }

void print_s(const char *str) { printf("%s", str); };

void print_d(int x) { printf("%d", x); }

void exit_proc() { return; }