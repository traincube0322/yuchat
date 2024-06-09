#include "yuchat_clnt.h"

void	*send_msg(void *arg);
void	*recv_msg(void *arg);
void	make_msg(char *input, msg_t *msg);
void	remove_newline(char *dest, const char *src);

char name[NAME_SIZE] = "[DEFAULT]";

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void *thread_return;

	if (argc != 4) {
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	if (strlen(argv[3]) > NAME_SIZE - 1)
		error_handling("name is too long");

	sprintf(name, "%s", argv[3]);
	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	pthread_create(&snd_thread, NULL, send_msg, (void *)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);

	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	close(sock);
	return 0;
}

void *send_msg(void *arg)   // send thread main
{
	msg_t 	*msg;
	int sock = *((int *)arg);
	char input[NAME_SIZE + BUF_SIZE + 2];

	msg = (msg_t *)malloc(sizeof(msg_t));
	if (msg == NULL)
		error_handling("malloc error");
	memset(msg, 0, sizeof(msg_t));
	
	while(1)
	{
		fgets(input, BUF_SIZE, stdin);
		if(!strcmp(input, "q\n") || !strcmp(input, "Q\n")) 
		{
			free(msg);
			close(sock);
			exit(0);
		}
		make_msg(input, msg);
		write(sock, msg, sizeof(msg_t));
	}
	free(msg);
	return NULL;
}

void	*recv_msg(void *arg)			// read thread main
{
	int sock = *((int *)arg);
	int rd;
	char buffer[BUF_SIZE];

	while (1)
	{
		rd = read(sock, buffer, BUF_SIZE);
		if (rd == -1)
			return (void *)-1;
		printf("READ MSG!\n");
		printf("%s\n", buffer);
	}
	return NULL;
}

void	make_msg(char *input, msg_t *msg)
{
	time_t cur_time;
	char reciever[NAME_SIZE];
	char content[BUF_SIZE];
	char timestamp[TIME_SIZE];

	msg->type = BROADCAST;
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
				return;
			}
			input += name_len;
		}
	}
	remove_newline(content, input);

	cur_time = time(NULL);
	strftime(timestamp, sizeof(timestamp), "%H:%M", localtime(&cur_time));
	sprintf(msg->message, "[%s] %s [%s]", name, content, timestamp);
	msg->len = strlen(msg->message);
}

void	remove_newline(char *dest, const char *src)
{
	int len = 0;

	while (src[len] != '\0' && src[len] != '\n')
		len++;
	strncpy(dest, src, len);
	dest[len] = '\0';
}