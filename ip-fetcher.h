Copyright (c) 2015-2016  Joachim Nilsson <troglobit@gmail.com>

#ifndef IP_FETCHER_H
#define IP_FETCHER_H

#include <stdio.h>
#include <sys/socket.h>

int connectIpify(void);
int queryIpify(int sockfd, char *addr, socklen_t len);
int disconnectIpify(int sockfd);
int fetchIp(char *addr, socklen_t len);


#endif // IP_FETCHER_H
