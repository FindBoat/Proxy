// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS489 PA2
//
// File:        LogUtils.h
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// Description: This File provides several method for recording logs

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sstream>
using namespace std;

#ifndef _LOGUTILS_H
#define _LOGUTILS_H

bool can_log = true;


// Print error, example: error("dequeue failure");
void error(const char *msg) {
//    cerr << msg << endl;
    perror(msg);
    exit(0);
}

void log_d(const char *tag, const string msg) {
    // Add timestamp on the log
    time_t now = time(0);
    tm *dtPostDate = localtime(&now);
    char t[50] ={0};
    const char format[] = "%H:%M:%S"; 
    strftime(t, sizeof(t), format, dtPostDate);

    // Output the log
    if (strcmp(tag, "zhaohang") == 0 && can_log) {
	cout << t << " " << tag << ": " << msg << endl; 
	return;
    }

}

// Convert int to string
string to_string(int i) {
    std::stringstream out;
    out << i;
    return out.str();
}

#endif


