#include "Game.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cassert>
#include <random>

int main() {
	int sockfd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
	assert(sockfd != -1);
	std::cout << "sockfd " << sockfd << std::endl;
	int zero = 0;
	int one = 1;
	assert(setsockopt(sockfd, IPPROTO_IPV6, IPV6_V6ONLY, &zero, sizeof(zero)) == 0);
	assert(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == 0);
	assert(setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one)) == 0);
	struct sockaddr_in6 serv_addr{
		.sin6_family = AF_INET6,
		.sin6_port = htons(9123),
		.sin6_flowinfo = 0,
		.sin6_addr = in6addr_any,
		.sin6_scope_id = 0,
	};
	assert(bind(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == 0);
	assert(listen(sockfd, 2) == 0);
	struct sockaddr_in sock_client{};
	socklen_t client_sock_len = sizeof(sock_client);
	while (true) {
		int p1fd = accept(sockfd, (struct sockaddr*) &sock_client, &client_sock_len);
		std::cout << p1fd << std::endl;
		int p2fd = accept(sockfd, (struct sockaddr*) &sock_client, &client_sock_len);
		std::cout << p2fd << std::endl;

		if (!fork()) {
			close(sockfd);

			std::mt19937 random{std::random_device{}()};

			Connection p1{p1fd};
			Connection p2{p2fd};

			Game g{p1, p2, random};
			g.run();
			return 0;
		}
		close(p1fd);
		close(p2fd);
	}
}
