// Copyright (c) 2012, Hang Zhao. All Rights Reserved.
//
// EECS489 PA1
//
// File:        DNSCache.h
// Date:        24 Jan 2012
// Author:      Hang Zhao (zhaohang@umich.edu)
// UMID:        84158329
// Description: This is file provides cache for dns

#include "LogUtils.h"
#include <map>
using namespace std;

#ifndef _DNSCACHE_H
#define _DNSCACHE_H

#define CACHE_SIZE 100

static map<string, string> cache;

string search_dns_cache(string host) {
    map<string, string>::iterator it = cache.find(host);
    if (it != cache.end()) {
	log_d("zhaohang", string("get ") + host + string(" from cache"));
	return it->second;
    } else {
	log_d("zhaohang", string("can't get ") + host + string(" from cache"));
	return string("");
    }
}

void insert_cache(string host, string addr) {
    if (cache.size() == CACHE_SIZE) {
	map<string, string>::iterator it = cache.begin();
	cache.erase(it);
    }
    cache.insert(pair<string, string>(host, addr));
    log_d("zhaohang", string("insert ") + host + string(" to cache"));
}

#endif
