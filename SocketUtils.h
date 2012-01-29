// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS 489 PA2
//
// File:        SocketUtil.h
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This file provides several implementations of socket

#include "LogUtils.h"
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>

#ifndef _SOCKETUTILS_H
#define _SOCKETUTILS_H

const int BUFFER_SIZE = 1000000;
const int MAX_CONCURRENT = 10;



string dns(const char *host) {
    char serv_ip[INET_ADDRSTRLEN];
    struct hostent *hp = gethostbyname(host);
    if (hp == NULL || hp->h_addr_list[0] == NULL)
	error("DNS error");
    inet_ntop(hp->h_addrtype, hp->h_addr_list[0], serv_ip, sizeof(serv_ip));
    return string(serv_ip);
}

// this function create server at addr:port
// after this function should call listen
int create_server(const char *addr, const char *port) {
    int listenfd;
    struct sockaddr_in servaddr;
    // Create socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
	error("Openning socket Error");
    // Initial the address
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    if (!addr)
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    else
	inet_pton(AF_INET, addr, &servaddr.sin_addr);
    servaddr.sin_port = htons(atoi(port));
    // Bind the address & port
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	error("Binding Error");
    log_d("zhaohang", string("Create server @port:") + port);
    return listenfd;
}

// this function create connection to addr:port
int create_connection(const char *addr, const char *port) {
    int sockfd;
    struct sockaddr_in servaddr;
    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	error("Openning Socket Error");
    // Setting the address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port));
    inet_pton(AF_INET, addr, &servaddr.sin_addr);
    // Connect
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	error("Connection Error");
    log_d("zhaohang", string("Connected to ") + addr + string(":") + port);
    return sockfd;
}

// The function m_write is to write all the data in the buffer through the socket    int sockfd;    int sockfd;
// Input:  sockfd is the sending socket describer
//         buffer is the sending buffer
//         length is the size of the sending buffer
// Return: return the size of data sent
ssize_t m_write(int sockfd, const void *buffer, size_t length) {
    size_t left = length;
    ssize_t written = 0;
    char *buf;
    buf = (char *) buffer;
    while (left > 0) {
	if ((written = write(sockfd, buf, left)) <= 0) {
	    if (written < 0 && errno == EINTR)
		written = 0; // Write again
	    else {
		perror("Writting Error");
		//break;
		written = 0;
	    }
	    
	}
	left -= written;
	buf += written;
    }
    return length;
}


// The function m_read is to read all the data in the buffer through the socket
// Input:  sockfd is the sending socket describer
//         buffer is the sending buffer
//         length is the size of the sending buffer
// Return: The size of data read
ssize_t m_read(int sockfd, void *buffer, size_t length) {
    size_t left = length;
    ssize_t nread = 0;
    char *buf;
    buf = (char *) buffer;
    while (left > 0) {
	if ((nread = read(sockfd, buf, left)) < 0) {
	    if (errno == EINTR)
		nread = 0; // Read again
	    else
		error("Reading error");
	} else if (nread == 0)
	    break; // EOF
	left -= nread;
    	buf += nread;
    }

    return (length - left);
}

int m_send(int sockfd, string mes) {
    int l = mes.length();
    char buf[l];
    m_write(sockfd, mes.c_str(), l);
}

string m_recv(int sockfd) {
    char buf[BUFFER_SIZE];
    m_read(sockfd, buf, BUFFER_SIZE);
    return string(buf);
}

#endif
