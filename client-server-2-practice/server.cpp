#include<iostream>
#include<cstring>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<assert.h>

const size_t k_max_msg = 4096;

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    int val = 1;

    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
}