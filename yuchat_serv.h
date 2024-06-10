/*
* Filename : yuchat_serv.h
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#ifndef YUCHAT_SERV_H
#define YUCHAT_SERV_H

#include "yuchat.h"
#include <netinet/in.h>
#include <time.h>

#define MAX_CLNT 256

typedef struct clnt_socks_s {
	int sock;
	char name[NAME_SIZE];
	struct clnt_socks_s	*next;
}				clnt_socks_t;


clnt_socks_t* create_node(int sock, const char* name);
void delete_node(clnt_socks_t** head_ref, int sock);
void append_node(clnt_socks_t **head_ref, int sock, const char *name);
clnt_socks_t *find_sock(clnt_socks_t *head, const char *name);
//clnt_socks_t *find_sock(clnt_socks_t *head, int sock);

#endif