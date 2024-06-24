/*
* Filename : yuchat_clnt.h
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#ifndef YUCHAT_CLNT_H
#define YUCHAT_CLNT_H

#include "yuchat.h"

int		make_msg(char *input, msg_t *msg, char *name);
void	remove_newline(char *dest, const char *src);

#endif