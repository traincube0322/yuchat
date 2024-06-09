#include "yuchat_clnt.h"

void	*send_msg(void *arg);
void	*recv_msg(void *arg);

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
	time_t	cur_time;
	int sock = *((int *)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	char input[NAME_SIZE + BUF_SIZE + 2];

	msg = (msg_t *)malloc(sizeof(msg_t));
	if (msg == NULL)
		error_handling("malloc error");
	memset(msg, 0, sizeof(msg_t));

	strcpy(msg->sender, name);
	while(1)
	{
		fgets(input, BUF_SIZE, stdin);
		if(!strcmp(input, "q\n") || !strcmp(input, "Q\n")) 
		{
			free(msg);
			close(sock);
			exit(0);
		}
		if (input[0] == '@')
		{
			msg->type = UNICAST;
			
		}
		else
		{
			msg->type = BROADCAST;
			strcpy(msg->content, input);
			msg->len = strlen(input);
			write(1, msg->content, strlen(msg->content));
		}
		cur_time = time(NULL);
		strftime(msg->timestamp, sizeof(msg->timestamp), "%H:%M", localtime(&cur_time));
		write(sock, msg, sizeof(msg_t));
		printf("example print msg\n");
		print_msg(1, msg);
	}
	free(msg);
	return NULL;
}

void *recv_msg(void *arg)			// read thread main
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	msg_t *msg;

	while (1)
	{
		if (read(sock, msg, sizeof(msg_t)) == -1)
			return (void *)-1;
		print_msg(1, msg);
	}
	return NULL;
}

// rcv format : [sender] very long msg [HH:MM]
