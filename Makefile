#
# Filename : yuchat.h
# Author : 서윤교 <tjdbsry1010@yu.ac.kr>
# Created : 2024-06-08
# Updated : 2024-06-08
#

SERV = yuchat_serv
CLNT = yuchat_clnt

CC = gcc
LDFLAGS = -lpthread

OBJS = yuchat.c

OBJS_SERV = chat_serv.c \
			yuchat_serv.c

OBJS_CLNT = chat_clnt.c \
			yuchat_clnt.c

all : ${SERV} ${CLNT}

${SERV} : 
	${CC} ${OBJS} ${OBJS_SERV} -o ${SERV} ${LDFLAGS}

${CLNT} : 
	${CC} ${OBJS} ${OBJS_CLNT} -o ${CLNT} ${LDFLAGS}

clean :
	rm ${SERV} ${CLNT}

re : clean all

.PHONY : all clena re