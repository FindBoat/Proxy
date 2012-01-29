// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS 489 Homework 1
//
// File:        client.cpp
// Date:        2012-01-14
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This file is the implementation of the client. The client receives
//              text messages from standard input and sends it to the server. And
//              the end of the message is marked by a control sequence which
//              corresponds to hitting ENTER twice or if the client reads messages
//              from a file it reaches EOF without seeing control sequence.

#include <arpa/inet.h>
#include "SocketUtils.h"

using namespace std;

// flag is false if the client calls the input function at the first time.
// This flag is for the input function to judge if there's already an ENTER in the input.
bool flag = false;
// The input function is called by the client to send messages from standard input.
// fp is the input stream and the sockfd is the client socket describer
void input(int sockfd) {
    // char sendBuf[BUFFER_SIZE];
    // while (!feof(fp)) {
    // 	fgets(sendBuf, BUFFER_SIZE, fp);
    // 	if (sendBuf[0] == '\n' && flag)
    // 	    break;
    // 	// mWrite is declared in head.h
    // 	mWrite(sockfd, sendBuf, BUFFER_SIZE);
    // 	bzero(sendBuf, BUFFER_SIZE); // sometimes there're some errors without this line
    // 	flag = true;
    // }
//    char *buf = "GET http://www.google.com/ HTTP/1.0\r\nAccept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Charset:GBK,utf-8;q=0.7,*;q=0.3\r\nAccept-Encoding:gzip,deflate,sdch\r\nAccept-Language:en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4\r\nConnection:keep-alive\r\nCookie:rememberme=true; HSID=AYfsL4G5xbAe-f3Pv; APISID=zZBzrrBYATuKi7qe/APao_B1b1u-Po8vo-; SS=DQAAALoAAABfdrSPgQs5iJl8BvFKilgP2BsqCZgcuvXDLMVLUQvf_lt34f_0mHOZeolT308YVP3eep1G6AchJdfQ0UsByaZu3d-5F8pYvTgIX54t_YImGaudXBsk33vyoFuYuFyuJHYQO7iY5fp5KZW02Ym3p2TGISBMLvpNVgPMT9_5vMyNOoEMnIbWEVSf76SyrBfJ4GHi6miXF2wFqDzYVCmE1-JNJqsV-NtDc591GKtbYtC_9uZsZEOT16Hhh7V99ysCMU8; NID=56=dgKGQhZZcoBAgWFIqfgYK51XEmXYJXOGqIArYvm-q66o7KIDoLvlDI4MiA75RJtefBJzRW53U3-6MwV-_mPnO-K4jBysn5AAQK9wKD6kFilwXCr2uXQddXRF2pdGGO13; PREF=ID=ab7a33fe77816d42:U=5e02a787b916f714:FF=0:NW=1:TM=1325419752:LM=1327518966:GM=1:S=TzX79YhgMJ76HCm9; SID=DQAAALgAAACMmX4ve_T1p_sR7t45Y4OeOlPvubS6RyI5rQPf2pxepgAOxRZE31OfqLEejEniJv7o8CjeW3KNdMp1Rvqp0vBnVTlI2FJ9EwIu58e506WwG2NE2YgGSTa-wPBXqbXtPhT560yJrnflKOZIdl2Y7AbQ_Ki3u15vzoiWRuyfPhPn9Ryn0FxzTdvkpkn2zZOy_AVh_oFrfRsrn1oYU5ONEf7hFKiUhuwISKNn3chpxVnNFy086mDnOk0mJsdQZy3Lg08\r\nHost:www.google.com\r\nUser-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.77 Safari/535.7\r\n\r\n";
//    char *buf = "GET http://www.google.com/ HTTP/1.0\r\nAccept:text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\nAccept-Charset:GBK,utf-8;q=0.7,*;q=0.3\r\nAccept-Encoding:gzip,deflate,sdch\r\nAccept-Language:en-US,en;q=0.8,zh-CN;q=0.6,zh;q=0.4\r\nHost:www.google.com\r\nUser-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.77 Safari/535.7\r\n\r\n";
    char *buf = "GET http://www.yahoo.com/ HTTP/1.0\r\n\r\n";
//    char *buf = "POST http://www.google.com/ HTTP/1.0\r\n\r\n";
    char b[65535];
    strcpy(b, buf);
    m_write(sockfd, b, 65535);

				       
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
	fprintf(stderr, "usage %s hostname port\n", argv[0]);
	exit(0);
    }
        
    int sockfd;
    struct sockaddr_in servaddr;
    // Create the socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	error("Openning Socket Error");
    // Setting the address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    // Connect
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	error("Connection Error");
    // Input
    input(sockfd);
    char recv[65535];
    m_read(sockfd, recv, 65535);
    fputs(recv, stdout);
    close(sockfd);
    return 0;
}
