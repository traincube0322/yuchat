#include "yuchat_serv.h"

void * handle_clnt(void *arg);
void send_msg(msg_t *msg, int cur_sock);
int	read_msg(int clnt_sock, msg_t *msg);
void delete_clnt(int clnt_sock);

clnt_socks_t *clnt_head;
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
	
		pthread_create(&t_id, NULL, handle_clnt, (void *)&clnt_sock);
		pthread_detach(t_id);
		printf("Connected client IP: %s \n", inet_ntoa(clnt_adr.sin_addr));
	}
	close(serv_sock);
	return 0;
}

void *handle_clnt(void *arg)
{
	int 	clnt_sock = *((int *)arg);
	char	clnt_name[NAME_SIZE];
	int 	name_len;
	msg_t	*msg;

	if ((name_len = read(clnt_sock, clnt_name, NAME_SIZE)) == -1)
	{
		perror("read error!\n");
		return (void *)-1;
	}
	printf("Hello, [%s]\n", clnt_name);

	pthread_mutex_lock(&mutx);
	append_node(&clnt_head, clnt_sock, clnt_name);
	pthread_mutex_unlock(&mutx);

	msg = (msg_t *)malloc(sizeof(msg));
	if (msg == NULL)
		error_handling("malloc error");

	while (read_msg(clnt_sock, msg) != 0)
		send_msg(msg, clnt_sock);

	pthread_mutex_lock(&mutx);
	delete_node(&clnt_head, clnt_sock);
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
		for (clnt_socks_t *ptr = clnt_head; ptr != NULL; ptr = ptr->next)
		{
			if (ptr->sock == cur_sock)
				continue;
			write(ptr->sock, msg->message, msg->len);
		}
	}
	if (msg->type == UNICAST)
	{
		clnt_socks_t *ptr = find_sock(clnt_head, msg->receiver);
		if (ptr == NULL)
			write(cur_sock, "[System] Can't find the name\n", 30);
		else
		{
			write(ptr->sock, MAGENTA, strlen(MAGENTA));
			write(ptr->sock, msg->message, msg->len);
			write(ptr->sock, RESET, strlen(RESET));
		}
	}
	pthread_mutex_unlock(&mutx);
}

int	read_msg(int clnt_sock, msg_t *msg)
{
	int rd;
	memset(msg, 0, sizeof(msg_t));
	rd = read(clnt_sock, msg, sizeof(msg_t));
	return rd;
}

