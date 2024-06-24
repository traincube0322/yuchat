/*
* Filename : yuchat_clnt.c
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#include "yuchat_clnt.h"

int	make_msg(char *input, msg_t *msg, char *name)
{
	time_t cur_time;
	char reciever[NAME_SIZE];
	char content[BUF_SIZE];
	char timestamp[TIME_SIZE];

	msg->type = BROADCAST;
	// 1:1 대화 기능
	if (input[0] == '@')
	{
		msg->type = UNICAST;
		input++;
		char *space = strchr(input, ' ');
		if (space != NULL)
		{
			int name_len = space - input;
			if (name_len < NAME_SIZE)
			{
				strncpy(msg->receiver, input, name_len);
				msg->receiver[name_len] = '\0';
			}
			else
			{
				printf("ERROR : Name is too long\n");
				return -1;
			}
			input += name_len;
		}
	}
	remove_newline(content, input);

	// 시간 정보 저장.
	cur_time = time(NULL);
	strftime(timestamp, sizeof(timestamp), "%H:%M", localtime(&cur_time));
	sprintf(msg->message, "[%s] %s [%s]", name, content, timestamp);
	msg->len = strlen(msg->message);
	return 0;
}

// 입력받은 메시지의 개행 문자를 제거
void	remove_newline(char *dest, const char *src)
{
	int len = 0;

	while (src[len] != '\0' && src[len] != '\n')
		len++;
	strncpy(dest, src, len);
	dest[len] = '\0';
}