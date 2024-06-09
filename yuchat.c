/*
* Filename : yuchat.c
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#include "yuchat.h"

void	error_handling(char *msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
