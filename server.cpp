#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <csignal>

volatile sig_atomic_t stop = 0;

void handle_sigint(int) {
    stop = 1;
}



int main(){
    signal(SIGINT, handle_sigint);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;// = new sockaddr;

    //can use inet_pton(AF_INET, "192.168.1.55", &addr.sin_addr);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(server_fd, (struct sockaddr *)&addr, sizeof(addr))<0){
        close(server_fd);
        throw std::runtime_error("bind didn't work");
    }

    listen(server_fd, 2);
    sockaddr_in client;
    socklen_t len = sizeof(client);
    //use fcntl(sockfd, F_SETFL, O_NONBLOCK); for nonblocking
    while(!stop){
        int client_fd = accept(server_fd, (struct sockaddr *)&client, &len);
        char buffer[1024];
        int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        std::cout<<buffer;
    }
    
    close(server_fd);



}