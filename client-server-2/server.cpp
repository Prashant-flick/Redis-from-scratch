#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>

const size_t k_max_msg = 4096;

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

static void die(const char* msg) {
    int err = errno;
    fprintf(stderr, "[%d] %s\n", err, msg);
    abort();
}

static int32_t read_full(int fd, char *buf, size_t n) {
    char *start = buf;
    size_t total = n;
    size_t offset = 0;

    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1;
        }

        assert((size_t)rv <= n);

        // 1️⃣ how many bytes read this iteration
        fprintf(stderr, "[read_full] read() returned %ld bytes\n", rv);

        // 2️⃣ dump EXACT bytes just read
        fwrite(buf, 1, rv, stderr);
        fprintf(stderr, " [read_full] bytes: \n");

        // 3️⃣ dump buffer state so far
        fprintf(stderr, "[read_full] buffer so far (%zu/%zu): ",
                offset + rv, total);
        fwrite(start, 1, offset + rv, stderr);
        fprintf(stderr, "\n\n");

        n -= (size_t)rv;
        // this will move the bug to n-bytes ahead, because n bytes are already added to the buf by kernel
        // when called the read method      
        buf += rv;
        offset += rv;
    }

    return 0;
}


static int32_t write_all(int fd, const char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t one_request(int connfd) {
    char rbuf[4 + k_max_msg];
    errno = 0;
    int32_t err = read_full(connfd, rbuf, 4);
    if (err) {
        msg(errno == 0 ? "EOF" : "read() error");
        return err;
    }

    uint32_t len = 0;
    memcpy(&len, rbuf, 4);
    if (len > k_max_msg) {
        msg("too long");
        return -1;
    }

    //request body
    err = read_full(connfd, &rbuf[4], len);
    if (err) {
        msg("read() error");
        return err;
    }

    fprintf(stderr, "client says: %.*s\n", len, &rbuf[4]);

    const char reply[] = "world";
    char wbuf[4 + sizeof(reply)];
    len = (uint32_t)strlen(reply);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], reply, len);
    return write_all(connfd, wbuf, 4+len); 
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
        printf("accept() returned connfd: %d\n", connfd);
        if (connfd < 0) {
            continue;
        }

        while(true) {
            int32_t err = one_request(connfd);
            printf("client call %d\n", err);
            if (err) {
                break;
            }
        }
        close(connfd);
    }

    return 0;
}

