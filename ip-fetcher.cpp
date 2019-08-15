Copyright (c) 2015-2016  Joachim Nilsson <troglobit@gmail.com>

#include "ip-fetcher.h"
#include <iostream>
#include <sstream>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define HTTP_REQUEST  "GET / HTTP/1.1\r\nHost:api.ipify.org\r\nConnection: close\r\n\r\n";

using namespace std;

int connectIpify(void)
{
    struct addrinfo *result, *rp;
    struct addrinfo hints;
    int rc, c, sockfd;

    // Obtain address or addresses that match the host
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_UNSPEC; // to use IPv6, replace with AF_INET6
    hints.ai_socktype = SOCK_STREAM; // for TCP connection

    rc = getaddrinfo("api.ipify.org", "http", &hints, &result);

    if (rc || !result)
    {
        //qDebug(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return -1;
    }

    /* getaddrinfo() returns a list of address structures.
    Try each address until the socket is connected to the server's address.*/
    for (rp = result; rp; rp = rp->ai_next) {
        // create an endpoint for communication
        // protocol is given as 0 since only a single protocol exists to support this scoket type
        sockfd = socket(rp->ai_family, rp->ai_socktype, 0);
        //Upon successful completion, socket() shall return a non-negative integer
        if (sockfd == -1)
            continue;

        // pointer to socket address is provided in ai_addr field
        // ai_addrlen is the length of the socket address in bytes
        c = connect(sockfd, result->ai_addr, result->ai_addrlen); // returns zero if connection is successful
        if (c==0)
            break;

        close(sockfd);
        }
    if (rp == NULL) {  // No address succeeded
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    return sockfd;
}

int queryIpify(int sockfd, char *addr, socklen_t len)
{
    const char *req = HTTP_REQUEST;
    size_t s,r;
    char tmp[sizeof(struct in6_addr)];
    char buf[512], *ptr;
    int domain;

    s = send(sockfd, req, strlen(req), 0);
    if (s == -1)
        return -1;

    r = recv(sockfd, buf, sizeof(buf), 0);
    if (r == -1)
        return -1;

    ptr = strstr(buf, "200 OK");   //200 OK is success status
    if (!ptr)
        return 1;

    ptr = strstr(ptr, "\r\n\r\n");
    if (!ptr)
        return 1;
    ptr += 4;

    domain = AF_INET;
    if (!inet_pton(domain, ptr, tmp)) {
        domain = AF_INET6;
        if (!inet_pton(domain, ptr, tmp))
            return 1;
    }

    if (!inet_ntop(domain, tmp, addr, len))
        return 1;

    return 0;
}

int disconnectIpify(int sockfd)
{
    shutdown(sockfd, SHUT_RDWR);
    return close(sockfd);
}

int fetchIp(char *addr, socklen_t len)
{
    int sockfd, ret;

    sockfd = connectIpify();
    ret    = queryIpify(sockfd, addr, len);
    ret   |= disconnectIpify(sockfd);

    return ret;
}
