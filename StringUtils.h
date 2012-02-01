// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS 489 PA2
//
// File:        StringUtils.h
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This file provides several methods for string manipulation
#include "LogUtils.h"
using namespace std;

#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H

#define MAX_LENGTH 65535
#define MAX_SPIT 30

string trim(string s) {
    s.erase(s.find_last_not_of(" ")+1); 
    return s.erase(0,s.find_first_not_of(" ")); 
}

string to_lowercase(string s) {
    char str[MAX_LENGTH];
    strcpy(str, s.c_str());
    int n = s.length();
    for (int i = 0; i < n; i++) {
	if (str[i] >= 'A' && str[i] <='Z')
	    str[i] -= 'A' - 'a';
    }
    return string(str);
}

string to_uppercase(string s) {
    char str[MAX_LENGTH];
    strcpy(str, s.c_str());
    int n = s.length();
    for (int i = 0; i < n; i++) {
	if (str[i] >= 'a' && str[i] <='z')
	    str[i] += 'A' - 'a';
    }
    return string(str);
}

bool is_path(string uri) {
    return (uri.at(0) == '/');
}

// change "\r" to "\r\n"
string format_request(string req) {
    int index = req.find_first_of("\r");
    while (index != -1) {
	if (req[index + 1] != '\n')
	    req = req.replace(index, 1, "\r\n");
	index = req.find_first_of("\r", index + 1);
    }
    return req;
}

string format_url(string url) {
    if (is_path(url))
	return url;
    if (url.substr(0, 3) == "www") // www.google.com/
	url = string("http://") + url;
    return url;
}

bool split(string s, string d, string *res, int *num) {
    string str(s);
    int k = 0;
    int index = str.find_first_of(d);
    while (index != -1) {
	string part = str.substr(0, index);
	if (part.length() != 0) {
	    if (k == MAX_SPIT)
		return false;
	    res[k++] = part;
	}
	if (index + d.length() >= str.length())
	    break;
	str = str.substr(index + d.length(), str.length());
	index = str.find_first_of(d);
    }
    if (str.length() != 0)
	res[k++] = str;
    *num = k;
    return true;
}

bool is_number(string s) {
    int n = s.length();
    for (int i = 0; i < n; i++) {
	if (s[i] < '0' || s[i] > '9')
	    return false;
    }
    return true;
}

#endif
