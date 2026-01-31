#include<stdio.h>
#include<stdlib.h>
#include<sys/select.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<iostream>
 
using namespace std;

#define PORT 1234
#define MAX_CLIENTS 10

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(listen_fd, 5);

    int clients[MAX_CLIENTS];
    for (int i=0; i < MAX_CLIENTS; i++) clients[i] = -1;

    cout << "server is listening on PORT: " << PORT << endl;

    while(1) {
        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(listen_fd, &readfds);
        int max_fd = listen_fd;

        // add client sockets
        for (int i=0; i<MAX_CLIENTS; i++) {
            if (clients[i] != -1) {
                FD_SET(clients[i], &readfds);
                if (clients[i] > max_fd) {
                    max_fd = clients[i];
                }
            }
        }

        // EVENT LOOP
        select(max_fd+1, &readfds, NULL, NULL, NULL);

        // new connection
        if(FD_ISSET(listen_fd, &readfds)) {
            int client_fd = accept(listen_fd, NULL, NULL);
            printf("New Client Connection: fd=%d\n", client_fd);
            
            for (int i=0; i<MAX_CLIENTS; i++) {
                if (clients[i] == -1) {
                    clients[i] = client_fd;
                    break;
                }
            }
        }
        
        // client
        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = clients[i];
            if (fd != -1 && FD_ISSET(fd, &readfds)) {
                char buf[1024];
                int n = read(fd, buf, sizeof(buf));

                if (n <= 0) {
                    printf("Client disconnected: fd=%d\n", fd);
                    close(fd);
                    clients[i] = -1;
                } else {
                    printf("read is not ready for fd=%d\n", fd);
                    write(fd, buf, n);
                }
            }
        }
    }
    
    return 0;
}