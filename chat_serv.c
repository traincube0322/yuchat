#include "yuchat_serv.h"

void * handle_clnt(void *arg);
void send_msg(msg_t *msg, int cur_sock);
int	read_msg(int clnt_sock, msg_t *msg);
void delete_clnt(int clnt_sock);
void	print_msg(int sock, msg_t *msg);

int clnt_cnt = 0;
int clnt_socks[MAX_CLNT];
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	pthread_mutex_init(&mutx, NULL);
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
		error_handling("bind() error");
	if (listen(serv_sock, 5) == -1)
		error_handling("listen() error");

	while (1)
	{
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);
		clnt_socks[clnt_cnt++] = clnt_sock;
		pthread_mutex_unlock(&mutx);
	
		pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void *handle_clnt(void *arg)
{
	int 	clnt_sock = *((int* )arg);
	msg_t	*msg;

	msg = (msg_t *)malloc(sizeof(msg));
	if (msg == NULL)
		error_handling("malloc error");

	while (read_msg(clnt_sock, msg) != -1)
	{
		printf("msg from %dsock\n", clnt_sock);
		send_msg(msg, clnt_sock);
	}
	
	pthread_mutex_lock(&mutx);
	delete_clnt(clnt_sock);
	pthread_mutex_unlock(&mutx);

	free(msg);
	close(clnt_sock);
	return NULL;
}

void send_msg(msg_t *msg, int cur_sock)
{
	int i;

	pthread_mutex_lock(&mutx);
	if (msg->type == BROADCAST)
	{
		//write(1, msg->content, msg->len);
		for (i = 0; i < clnt_cnt; i++)
		{
			if (clnt_socks[i] == cur_sock)
				continue;
			write(clnt_socks[i], msg, sizeof(msg_t));
			printf("send to %d\n", clnt_socks[i]);
		}
		print_msg(1, msg);
	}
	if (msg->type == UNICAST)
	{
		printf("UNICAST is unsported yet... sorry\n");
	}
	pthread_mutex_unlock(&mutx);
}

int	read_msg(int clnt_sock, msg_t *msg)
{
	memset(msg, 0, sizeof(msg_t));
	read(clnt_sock, msg, sizeof(msg_t));
	return 0;
}

void delete_clnt(int clnt_sock)
{
	for (int i = 0; i < clnt_cnt; i++)   // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			while(i < clnt_cnt - 1)
			{
				clnt_socks[i] = clnt_socks[i + 1];
				i++;
			}
			break;
		}
	}
	clnt_cnt--;
}

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