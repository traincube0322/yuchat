/*
* Filename : yuchat_clnt.c
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#include "yuchat.h"

void	print_msg(int sock, msg_t *msg)
{
	char name[NAME_SIZE + 3];
	char time_stamp[TIME_SIZE + 3];

	if (msg->type == UNICAST)
		write(sock, MAGENTA, strlen(MAGENTA));

	sprintf(name, "[%s] ", msg->sender);
	sprintf(time_stamp, " [%s]", msg->timestamp);

	write(sock, name, strlen(name));
	write(sock, msg->content, msg->len);
	write(sock, time_stamp, strlen(time_stamp));
	write(sock, "\n", 1);
	write(sock, RESET, strlen(RESET));
}