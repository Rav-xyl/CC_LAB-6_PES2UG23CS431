#include <iostream>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    char hostname[1024];
    gethostname(hostname, sizeof(hostname));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);

    std::cout << "Server listening on port 8080 (hostname: " << hostname << ")" << std::endl;

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);

        std::string body = "Served by backend: ";
        body += hostname;
        body += "\n";

        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: " + std::to_string(body.length()) + "\r\n"
            "Connection: close\r\n"
            "\r\n" + body;

        send(new_socket, response.c_str(), response.size(), 0);
        close(new_socket);
    }
}
