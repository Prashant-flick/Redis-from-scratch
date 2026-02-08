#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<arpa/inet.h>

using namespace std;

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {0};
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(1234);
}