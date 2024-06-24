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
	char buffer[BUF_SIZE];

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

	write(sock, name, NAME_SIZE);
	int rd;
	if ((rd = read(sock, buffer, BUF_SIZE)) == 0)
	{
		printf("ERROR : Socket is closed\n");
		return 0;
	}
	pthread_create(&snd_thread, NULL, send_msg, (void *)&sock);
	pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);

	pthread_join(snd_thread, &thread_return);
	pthread_join(rcv_thread, &thread_return);

	close(sock);
	return 0;
}

// 서버에게 msg_t에 정보를 채워 전송
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
		if (make_msg(input, msg, name) == 0)
			write(sock, msg, sizeof(msg_t));
	}
	free(msg);
	return NULL;
}

// 서버로부터 메시지를 수신
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
		if (rd == 0)
			break;
		buffer[rd] = '\0';
		write(1, buffer, strlen(buffer));
		write(1, "\n", 1);
	}
	return NULL;
}
