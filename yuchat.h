/*
* Filename : yuchat.h
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#ifndef YUCHAT_H
#define YUCHAT_H

#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

// ANSI Escape Code : Color
#define RESET	"\033[0m"
#define RED		"\033[31m"
#define GREEN	"\033[32m"
#define YELLOW	"\033[33m"
#define BLUE	"\033[34m"
#define MAGENTA	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"

#define BUF_SIZE	512
#define NAME_SIZE	20
#define MSG_SIZE	128
#define TIME_SIZE	6

#define BROADCAST	0
#define UNICAST 	1

typedef struct msg_s
{
	int		type;
	int		len;
	char	receiver[NAME_SIZE];
	char	message[NAME_SIZE + MSG_SIZE + TIME_SIZE + 7];
}	msg_t;

// functions
void	error_handling(char *msg);


#endif