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

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2)
	 error("Invalid input");
    // create socket for listenning
    int listenfd, connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    listenfd = create_server(NULL, argv[1]);
    listen(listenfd, MAX_CONCURRENT);
    // accept the connection
    clilen = sizeof(cliaddr);
    connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
    if (connfd < 0)
	error("Connection Error");
    log_d("zhaohang", "Connected with client");
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    int count;
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    m_read(connfd, buffer, BUFFER_SIZE);

    // create HttpRequest
    HttpRequest request(buffer);

    if (!request.is_valid()) {
	//TODO 
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    // redirect
    // DNS
    struct hostent *hp = gethostbyname(request.get_host().c_str());
    if (hp == NULL || hp->h_addr_list[0] == NULL)
	error("DNS error");
    char serv_ip[INET_ADDRSTRLEN];
    inet_ntop(hp->h_addrtype, hp->h_addr_list[0], serv_ip, sizeof(serv_ip));

    // connect to server
    int proxyfd = create_connection(serv_ip, request.get_port().c_str());
    char req_message[BUFFER_SIZE];
    strcpy(req_message, request.get_request_message().c_str());
    log_d("zhaohang", string("send request: ") + req_message);
    m_write(proxyfd, req_message, BUFFER_SIZE);
    bzero(buffer, BUFFER_SIZE);
    m_read(proxyfd, buffer, BUFFER_SIZE);
    fputs(buffer, stdout);

    close(proxyfd);
    // send back to client
    				   
    m_write(connfd, buffer, BUFFER_SIZE);

    close(listenfd);

    return 0;

}
