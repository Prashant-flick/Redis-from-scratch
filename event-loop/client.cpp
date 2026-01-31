#include<iostream>
#include<stdlib.h>
#include<sys/select.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>

using namespace std;

#define PORT 1234

int main() {
    int fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(PORT);
    addr.sin_family = AF_INET;

    connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    char msg[] = "hello from client\n";
    write(fd, msg, strlen(msg));

    char buf[1024];
    int n = read(fd, buf, sizeof(buf));
    write(1, buf, n);

    close(fd);
}