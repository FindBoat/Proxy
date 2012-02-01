CC=g++
CFLAGS= -m32 -g -Wall -Werror -Wno-write-strings

all: proxy

proxy: proxy.cc LogUtils.h HttpRequest.h StringUtils.h SocketUtils.h
	$(CC) $(CFLAGS) -o proxy.o -c proxy.cc 
	$(CC) $(CFLAGS) -o proxy proxy.o -lpthread
clean:
	rm -f proxy *.o