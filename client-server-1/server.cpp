#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char* msg) {
    perror(msg);
    exit(1);
}

static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    // getting a socket handle
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Socket created with fd: %d\n", fd);
    int val = 1;
    // setting socket options
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    // binding socket
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);        // port
    addr.sin_addr.s_addr = htonl(0);    // wildcard IP 0.0.0.0
    //bind
    int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
    printf("bind() returned: %d\n", rv);
    if (rv != 0) {
        die("bind()");
        printf("Error binding socket\n");
    }
    
    // listening on socket
    rv = listen(fd, SOMAXCONN);
    printf("listen() returned: %d\n", rv);
    if (rv != 0) {
        die("listen()");
        printf("Error listening on socket\n");
    }

    while (true) {
        // accepting connections
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *)&client_addr, &addrlen);
        printf("addrlen after accept: %d\n", addrlen);
        printf("accept() returned connfd: %d\n", connfd);

        if (connfd < 0) {
            continue; // error
        }
        do_something(connfd);
        close(connfd);
    }
}

