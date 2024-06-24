/*
* Filename : yuchat_serv.c
* Author : 서윤교 <tjdbsry1010@yu.ac.kr>
* Created : 2024-06-08
* Updated : 2024-06-08
*/

#include "yuchat_serv.h"

// 노드 생성
clnt_socks_t* create_node(int sock, const char* name)
{
	clnt_socks_t *new_node = (clnt_socks_t *)malloc(sizeof(clnt_socks_t));
	if (new_node == NULL) {
		printf("malloc error");
		return NULL;
	}
	new_node->sock = sock;
	strncpy(new_node->name, name, NAME_SIZE - 1);
	new_node->name[NAME_SIZE - 1] = '\0';
	new_node->next = NULL;
	return new_node;
}

// 노드 삭제
void delete_node(clnt_socks_t **head, int sock)
{
	clnt_socks_t *temp = *head;
	clnt_socks_t *prev = NULL;

	// 헤드 노드를 삭제
	if (temp != NULL && temp->sock == sock) {
		*head = temp->next;
		free(temp);
		return;
	}

	while (temp != NULL && temp->sock != sock) {
		prev = temp;
		temp = temp->next;
	}

	// 노드 없음
	if (temp == NULL) return;

	prev->next = temp->next;
	free(temp);
}

// 가장 마지막에 노드 추가
void append_node(clnt_socks_t **head, int sock, const char *name)
{
	clnt_socks_t* new_node = create_node(sock, name);

	if (new_node == NULL)
		return;

	// 리스트가 비어 있는 경우
	if (*head == NULL) {
		*head = new_node;
		return;
	}

	clnt_socks_t *last = *head;
	while (last->next != NULL) {
		last = last->next;
	}
	last->next = new_node;
}

// name인 노드를 찾기
clnt_socks_t *find_sock(clnt_socks_t *head, const char *name)
{
	clnt_socks_t *ptr = head;

	while (ptr != NULL)
	{
		if (strcmp(ptr->name, name) == 0)
			return ptr;
		ptr = ptr->next;
	}
	return NULL;
}
