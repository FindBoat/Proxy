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
#include "StringUtils.h"
using namespace std;

#ifndef _HTTPREQUEST_H
#define _HTTPREQUEST_H

#define DEFAULT_PORT "80"
#define BAD_REQUEST "400"
#define NOT_IMPLEMENTED "501"
#define CRLF string("\r\n")

class HttpRequest {
private:
    string request_line, method, uri, version, host, port;
    string header_lines[MAX_SPIT];
    int header_num;
    bool validity;
    string status_code;

    void set_request_line();
    void parse_url();
    void set_valid();
    void set_invalid_code(string code);
    string get_error_message(string code);
    string format(string req);
public:
    HttpRequest(char* req);
    ~HttpRequest();
    string get_host();
    string get_port();
    bool is_valid();
    string get_request_message();
    string generate_error_reply();
};

HttpRequest::HttpRequest(char *r) {
    log_d("zhaohang", "Init Http Request...");
    string req(r);
    this->validity = true;
    req = format_request(req);
    string res[MAX_SPIT];
    int num;
    if (!split(req, CRLF, res, &num))
	error("Too many header lines");
    this->request_line = res[0];
    for (int i = 1; i < num; i++)
	this->header_lines[i - 1] = res[i];
    this->header_num = num - 1;
    log_d("zhaohang", "Set request line...");
    set_request_line();
    log_d("zhaohang", "Parse url...");
    parse_url();
    set_valid();
}

void HttpRequest::set_request_line() {
    if (this->validity) {
	string res[MAX_SPIT];
	int num;
	split(this->request_line, " ", res, &num);
	if (num != 3) {
	    set_invalid_code(BAD_REQUEST);
	    return;
	}
	this->method = to_uppercase(res[0]);
	this->uri = res[1];
	this->version = to_uppercase(res[2]);
	// set port #
	int colon = this->uri.find_last_of(":");
	if (colon == -1 || (colon != -1 && this->uri.substr(colon + 1, 2) == "//"))
	    this->port = DEFAULT_PORT;
	else { 
	    this->port = this->uri.substr(colon + 1, this->uri.length());
	    this->uri = this->uri.substr(0, colon); 
	}
	this->uri = format_url(this->uri);
	log_d("zhaohang", string("set method = ") + this->method);
	log_d("zhaohang", string("set uri = ") + this->uri);
	log_d("zhaohang", string("set port = ") + this->port);
	log_d("zhaohang", string("set version = ") + this->version);
    }
}

void HttpRequest::parse_url() {
    if (!this->validity)
	return;
    bool has_host = false;
    // find Host tag
    for (int i = 0; i < this->header_num; i++) {
	if (to_uppercase(this->header_lines[i].substr(0, 4)) == "HOST") {
	    // parse host and uri
	    has_host = true;
	    this->host = this->header_lines[i].substr(5, this->header_lines[i].length()); // 5 is for "Host:"
	    this->host = trim(this->host);
	    break;
	}
    }
    if (is_path(this->uri)) {
	if (!has_host)
	    set_invalid_code(BAD_REQUEST);
	return;		
    }
    int slash = this->uri.find_first_of("/", 7); // 7 is for "http://"
    if (slash == -1) {
	this->uri += "/";
	slash = this->uri.length() - 1;
    }
    if (!has_host) {
	this->host = this->uri.substr(7, slash - 7); // slash - 7 is the count
	if (this->header_num == MAX_SPIT)
	    error("Too many header lines");
	this->header_lines[this->header_num++] = string("Host: ") + this->host;
    }
    this->uri = this->uri.substr(slash, this->uri.length());
    log_d("zhaohang", string("set Host = ") + this->host + string(", uri = ") + this->uri + string(", port = ") + this->port);
}

void HttpRequest::set_valid() {
    if (!this->validity)
	return;
    if (this->method != "GET") {
	this->validity = false;
	this->status_code = NOT_IMPLEMENTED;
    }
    if (this->version == "HTTP/1.1" || this->version == "HTTP/0.9")
	this->version = "HTTP/1.0";
}

void HttpRequest::set_invalid_code(string code) {
    this->validity = false;
    this->status_code = code;
    log_d("zhaohang", string("Invalid: ") + code);
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

string HttpRequest::get_error_message(string code) {
    if (code == BAD_REQUEST)
	return string("Bad Request");
    if (code == NOT_IMPLEMENTED)
	return string("Not Implemented");
    error("Status code error");
    return string();
}

string HttpRequest::generate_error_reply() {
    if (!this->is_valid()) {
	string re = string("HTTP/1.0 ") + this->status_code + string(" ")
	    + get_error_message(this->status_code) + CRLF + CRLF;
	return re;
    }
    error("Request is valid");
    return string();
}

HttpRequest::~HttpRequest() {}

#endif
