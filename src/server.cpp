#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>  // sockaddr_in, INADDR_LOOPBACK
#include <arpa/inet.h>   // htons(), htonl()
#include <unistd.h>     // close()
#include <cerrno>
#include <cstring>
int main()
{

	//Create an IPv4 TCP socket
		int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (server_fd == -1)
	{
		std::cerr << "Failed to create socket\n";
		return 1;
	}

	std::cout<< "Socket created successfully. FD: " << server_fd<< '\n';

	sockaddr_in server_addr{};
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8081);
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	



	// 3. Bind the socket to the address and port
	if (bind(server_fd,
				reinterpret_cast<sockaddr*>(&server_addr),
				sizeof(server_addr)) == -1)
	{
		std::cerr << "bind() failed:"<<strerror(errno)<< '\n';
		close(server_fd);
		return 1;
	}

	std::cout << "Socket bound to 127.0.0.1:8080\n";

	if (listen(server_fd, 5) == -1)
	{
		std::cerr << "listen() failed: "
			<< strerror(errno) << '\n';
		close(server_fd);
		return 1;
	}

	std::cout << "Server is listening on port 8081\n";

	// 5. Accept one client connection
	while (true) 
	{
	int client_fd = accept(server_fd, nullptr, nullptr);

	if (client_fd == -1)
	{
		std::cerr << "accept() failed: "
			<< strerror(errno) << '\n';
		close(server_fd);
		return 1;
	}

	std::cout << "Client connected successfully!\n";

	char buffer[1024];
      while (true)
      {
	ssize_t bytes_received = recv(
			client_fd,
			buffer,
			sizeof(buffer) - 1,
			0
			);

	if (bytes_received == 0)
	{
		std::cout << "Client disconnected.\n";
		break;
	}

	if (bytes_received == -1)
	{
		std::cerr << "recv() failed: "
			<< strerror(errno) << '\n';
		break;
	}

	buffer[bytes_received] = '\0';

	std::cout << "Message from client: "
		<< buffer << '\n';


	const char* response = "Hello client! Message received.\n";

	ssize_t bytes_sent = send(
			client_fd,
			response,
			strlen(response),
			0
			);

	if (bytes_sent == -1)
	{
		std::cerr << "send() failed: "
			<< strerror(errno) << '\n';
		break;
	}
      }

      close(client_fd);
	}
	close(server_fd);

	return 0;
}
