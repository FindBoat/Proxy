// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS 489 PA2
//
// File:        HttpRequest.h
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This file provides the class HttpRequest

#include "LogUtils.h"
using namespace std;

#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#define MAX_HEADER_NUM 20

#define BAD_REQUEST "400"
#define NOT_IMPLEMENTED "501"

#define CRLF string("\r\n")

class HttpRequest {
private:
    string request_line;
    string method;
    string uri;
    string version;
    string host;
    string port;
    string header_lines[MAX_HEADER_NUM];
    int header_num;
    bool validity;
    string status_code;

    void set_request_line();
    void parse_url();
    void set_valid();
    void set_invalid_code(string code);
    bool is_path();
public:
    HttpRequest(const char* request);
    ~HttpRequest();
    string get_host();
    string get_port();
    bool is_valid();
    string get_request_message();
};

HttpRequest::HttpRequest(const char* request) {
    this->validity = true;
    string req(request);
    int length = req.length();
    int header_num = 0;
    bool flag = true; // to get the request line
    int crlf = req.find_first_of(CRLF, 0);
    while (crlf != 0) {
	if (crlf == -1) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	if (header_num == MAX_HEADER_NUM)
	    error("Too many header lines");
	if (flag) {
	    this->request_line = req.substr(0, crlf);
	    flag = false;
	} else
	    this->header_lines[header_num++] = req.substr(0, crlf);
	req = req.substr(crlf + 2, length);
	crlf = req.find_first_of(CRLF, 0);
    }
    log_d("zhaohang", string("request line: ") + this->request_line);
    log_d("zhaohang", string("header lines: "));
    for (int i = 0; i < header_num; i++)
	log_d("zhaohang", this->header_lines[i]);
    this->header_num = header_num;

    set_request_line();
    parse_url();
    set_valid();
    
}

HttpRequest::~HttpRequest() {
    
}

void HttpRequest::set_request_line() {
    string line = this->request_line;
    if (this->validity) {
	int space = line.find_first_of(" ", 0);
	if (space == -1) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	this->method = line.substr(0, space);
	line = line.substr(space + 1, line.length());
	space = line.find_first_of(" ", 0);
	if (space == -1) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	this->uri = line.substr(0, space);
	this->version = line.substr(space + 1, line.length());

	log_d("zhaohang", string("set method = ") + this->method);
	log_d("zhaohang", string("set uri = ") + this->uri);
	log_d("zhaohang", string("set version = ") + this->version);
    }

}
void HttpRequest::parse_url() {
    if (!this->validity)
	return;
    bool has_host = false;
    // find Host tag
    for (int i = 0; i < this->header_num; i++) {
	if (this->header_lines[i].substr(0, 4) == "Host") {
	    // parse host and uri
	    has_host = true;
	    this->host = this->header_lines[i].substr(5, this->header_lines[i].length()); // 5 is for "Host:"
	    // set uri
	    if (!is_path()) {
		int slash = this->uri.find_first_of("/", 7); // 7 is for "Http://"
		if (slash == -1) {
		    set_invalid_code(BAD_REQUEST);
		    return;
		}
		this->uri = this->uri.substr(slash, this->uri.length());
	    }
	    break;
	}
    }
    // if there's no host in header lines
    if (!has_host) {
	if (is_path()) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	// parse uri from host
	int slash = this->uri.find_first_of("/", 7); // 7 is for "Http://"
	if (slash == -1) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	this->host = this->uri.substr(7, slash - 7); // slash - 7 is the count
	this->uri = this->uri.substr(slash, this->uri.length());

    }

    // set port #
    int colon = this->uri.find_first_of(":", 0);
    if (colon != -1) {
	this->port = this->uri.substr(colon + 1, this->uri.length());
	this->uri = this->uri.substr(0, colon);
    } else
	this->port = "80";

    log_d("zhaohang", string("set Host = ") + this->host + string(", uri = ") + this->uri + string(", port = ") + this->port);

}

void HttpRequest::set_valid() {
    if (!this->validity)
	return;
    if (this->method != "GET" && this->method != "get") {
	this->validity = false;
	this->status_code = NOT_IMPLEMENTED;
    }
}

void HttpRequest::set_invalid_code(string code) {
    this->validity = false;
    this->status_code = code;
    log_d("zhaohang", string("Invalid: ") + code);
}

bool HttpRequest::is_path() {
    return (this->uri.at(0) == '/');
}

string HttpRequest::get_host() {
    return this->host;
}

string HttpRequest::get_port() {
    return this->port;
}

bool HttpRequest::is_valid() {
    return this->validity;
}

string HttpRequest::get_request_message() {
    string message = this->method + string(" ") + this->uri + string(" ")
	+ this->version + CRLF;
    for (int i = 0; i < this->header_num; i++)
	message += this->header_lines[i] + CRLF;
    message += CRLF;
    return message;
}

#endif
