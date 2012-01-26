#include "HttpRequest.h"
using namespace std;

int main() {
    char *r = "get http://www.google.com/a/b/c:8080 HTTP/1.0\r\nHost:www.google.com\r\nUser-Agent:Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.77 Safari/535.7\r\nAccept-Charset:GBK,utf-8;q=0.7,*;q=0.3\r\n\r\n";

    HttpRequest req(r);


}
