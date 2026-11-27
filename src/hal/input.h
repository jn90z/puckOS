#pragma once
#include <linux/input.h>
int puck_input_open(const char *name);
int puck_input_read(int fd, struct input_event *event);
