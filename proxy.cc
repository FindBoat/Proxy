// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS489 PA1
//
// File:        proxy.cc
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This is the main file for the proxy

#include "LogUtils.h"
#include "SocketUtils.h"
#include "HttpRequest.h"
#include <stdio.h>
#include <pthread.h>
using namespace std;

void *proxy(void *arg) {
    char buf[BUFFER_SIZE];
    log_d_sep("zhaohang", string("thread ") + to_string(pthread_self()) + string(" started"));
    int connfd = *((int *) arg);
    delete (int *) arg;
    try {
	pthread_detach(pthread_self());
	// receive data from client
	read(connfd, buf, BUFFER_SIZE);
	log_d_sep("zhaohang", string("Receive data from client:\n") + buf);
	// create HttpRequest
	log_d_sep("zhaohang", "Format Http Request...");
	HttpRequest request(buf);
	// reply invalid request
	if (!request.is_valid()) {
	    strcpy(buf, request.generate_error_reply().c_str());
	    m_write(connfd, buf, request.generate_error_reply().length());
	    close(connfd);
	    return 0;
	}
	// DNS
	string serv_ip = dns(request.get_host().c_str());
	// connect to server
	int proxyfd = create_connection(serv_ip.c_str(), request.get_port().c_str());
	log_d_sep();
	log_d_sep("zhaohang", string("Request data:\n") + request.get_request_message());
	strcpy(buf, request.get_request_message().c_str());
	m_write(proxyfd, buf, request.get_request_message().length());
	log_d_sep("zhaohang", "Request sent");
	int count;
	while ((count = m_read(proxyfd, buf, BUFFER_SIZE)) > 0) 
	    m_write(connfd, buf, count);
	log_d("zhaohang", string("thread ") + to_string(pthread_self()) + string(" finished"));
    } catch (int) {
	log_d("zhaohang", string("thread ") + to_string(pthread_self()) + string(" terminated"));
    }
    close(connfd);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
	 error("Invalid input");
    // create socket for listenning
    int listenfd;
    int *connfd;
    pthread_t tid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    try {
	listenfd = create_server(NULL, argv[1]);
	listen(listenfd, MAX_CONCURRENT);
	while (true) {
	    // accept the connection
	    char tmp[BUFFER_SIZE];
	    clilen = sizeof(cliaddr);
	    connfd = new int;
	    *connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
	    if (*connfd < 0)
		error("Connection Error");
	    log_d_sep("zhaohang", string("Connected with ") + inet_ntop(AF_INET, &cliaddr.sin_addr, tmp, sizeof(tmp))
		      + string(":") + to_string(ntohs(cliaddr.sin_port)));
	    pthread_create(&tid, NULL, &proxy, connfd);
	}
    } catch (int& ex) {
    }
    return 0;

}
