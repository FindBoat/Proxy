#include "SocketUtils.h"

using namespace std;



int main(int argc, char* argv[]) {
    int listenfd = create_server(NULL, "4445");
    int connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    listen(listenfd, MAX_CONCURRENT);
    while (true) {
	clilen = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
	if (connfd < 0)
	    error("Connection Error");
	cout << "connected" << endl;
    }
    
    return 0;
}
