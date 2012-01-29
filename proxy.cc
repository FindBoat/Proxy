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

int connfd;
void *proxy(void *arg) {
    log_d("zhaohang", string("thread ") + to_string(pthread_self()) + string(" started"));

    int connfd = *((int *) arg);
    delete (int *) arg;
    pthread_detach(pthread_self());

    // receive data from client
    log_d_sep();
    
    char r[BUFFER_SIZE];
    ppread(connfd, r, BUFFER_SIZE);
    string buffer(r);
//    string buffer = m_recv(connfd);
    
    log_d("zhaohang", string("Receive data from client:\n") + buffer);
    // create HttpRequest
    log_d_sep();
    log_d("zhaohang", "Format Http Request...");
    HttpRequest request(buffer);
    // reply invalid request
    if (!request.is_valid()) {
	m_send(connfd, request.generate_error_reply());
	return 0;
    }
    log_d_sep();
    // DNS
    string serv_ip = dns(request.get_host().c_str());
    // connect to server
    int proxyfd = create_connection(serv_ip.c_str(), request.get_port().c_str());
    log_d_sep();
    log_d("zhaohang", string("Request data:\n") + request.get_request_message());
    log_d_sep();

    m_send(proxyfd, request.get_request_message());

    log_d("zhaohang", "Request sent");
    log_d_sep();

    buffer = m_recv(proxyfd);
    m_send(connfd, buffer);
    
    log_d("zhaohang", "Send back to client");
    close(connfd);
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
	log_d("zhaohang", string("Connected with ") + inet_ntop(AF_INET, &cliaddr.sin_addr, tmp, sizeof(tmp))
	      + string(":") + to_string(ntohs(cliaddr.sin_port)));

	log_d_sep();
	pthread_create(&tid, NULL, &proxy, connfd);

    }
    return 0;

}
