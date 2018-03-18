#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main () {
    char server_address[] = "192.168.1.2", broadcast_address[] = "10.0.2.255";
    char message[] = "Zdravstvuyte from server";
    const int bool_on_off = 1;
    char *ip;
    int server_socket_fd, port = 7777, bytes, struct_len, setsockopt_res;
    struct sockaddr_in server_info;

    memset(&server_info, 0, sizeof(server_info));

    if (inet_aton(broadcast_address, (struct in_addr *)&server_info.sin_addr) == 0) {
    printf("Address of server is invalid!\n");
    return 0;
    }

    server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket_fd < 0) {
    printf("error in function socket()!\n");
    return 0;
    }

    setsockopt_res = setsockopt(server_socket_fd, SOL_SOCKET, SO_BROADCAST, &bool_on_off, sizeof(bool_on_off));
    printf("setsockopt_res = %d\n", setsockopt_res);
    if (setsockopt_res < 0) {
    //printf("error in function setsockopt()!\n");
    perror("error in function setsockopt(): ");
    return 0;
    }

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port);

    ip = inet_ntoa(server_info.sin_addr);
    //struct_len = sizeof(client_info);
    struct_len = sizeof(server_info);

    while (1) {
    //bytes = sendto(server_socket_fd, message, sizeof(message), 0, (struct sockaddr *)&client_info, struct_len);
    bytes = sendto(server_socket_fd, message, sizeof(message), 0, (struct sockaddr *)&server_info, struct_len);
    if (bytes < 0) {
        printf("error in function sendto(), sended %d bytes!\n", bytes);
    } else {
        printf("SERVER(%s:%d) sended %d bytes to clients\n", ip, ntohs(server_info.sin_port), bytes);
    }
    sleep(10);
    }
    close(server_socket_fd);
    return 0;
}