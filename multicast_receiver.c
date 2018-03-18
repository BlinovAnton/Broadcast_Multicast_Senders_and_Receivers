#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main () {
    int bytes, port = 7777, server_socket_fd, struct_len, setsockopt_res;
    char server_address[] = "192.168.1.2", multicast_address[] = "224.0.0.4",server_address1[] = "10.0.2.15";
    char buf_for_recv[100];
    struct sockaddr_in server_info, client_info;
    struct ip_mreq mmreq;

    memset(&server_info, 0, sizeof(server_info));

    if (inet_aton(multicast_address, (struct in_addr *)&server_info.sin_addr) == 0) {
	printf("Address of server is invalid!\n");
	return 0;
    }
    if (inet_aton(multicast_address, (struct in_addr *)&mmreq.imr_multiaddr) == 0) {
	printf("Address of mreq is invalid!\n");
	return 0;
    }
    if (inet_aton(server_address1, (struct in_addr *)&mmreq.imr_interface) == 0) {
	printf("Address of mreq is invalid!\n");
	return 0;
    }

    server_socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket_fd < 0) {
    printf("error in function socket()!\n");
    return 0;
    }

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(port);

    printf("mreq_multiaddr = %s\n", inet_ntoa((struct in_addr)mmreq.imr_multiaddr));
    printf("mreq_interface = %s\n", inet_ntoa((struct in_addr)mmreq.imr_interface));

    if (bind(server_socket_fd, (struct sockaddr *)&server_info, sizeof(server_info)) < 0) {
	perror("error in function bind()");
	close(server_socket_fd);
	return 0;
    }

    //struct_len = sizeof(mmreq);
    struct_len = sizeof(struct ip_mreq);
    setsockopt_res = setsockopt(server_socket_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mmreq, (socklen_t)struct_len);
    printf("setsockopt_res = %d\n", setsockopt_res);
    if (setsockopt_res < 0) {
	perror("error in function setsockopt()");
	return 0;
    }

    struct_len = sizeof(client_info);
    printf("address for bind - %s:%d\n", inet_ntoa((struct in_addr)server_info.sin_addr), ntohs(server_info.sin_port));
while (1) {
    printf("Hm...\t");
    bytes = recvfrom(server_socket_fd, buf_for_recv, 100, 0, (struct sockaddr *)&client_info, (socklen_t *)&struct_len);
    //bytes = recvfrom(server_socket_fd, buf_for_recv, 100, 0, NULL, NULL);
    if (bytes < 0) {
	printf("error in function recvfrom(), received %d bytes!\n", bytes);
    } else {
	printf("Receiveded %d bytes from bcast_sender (): %s\n", bytes, buf_for_recv);
    }
    sleep(5);
}
    close(server_socket_fd);
    return 0;
}