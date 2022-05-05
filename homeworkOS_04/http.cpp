#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>
namespace http{

	class Request{
		std::string path;
		std::string body;
		std::string method;
		std::string version;
		std::map<std::string, std::string> header;
		// Content length, content type, accept type (?)
	};

	class Response{
		std::string body;
		std::string version;
		std::string statusNum;
		std::map<std::string, std::string> header;
		// Content length, content type
	};

	class Server{
		int port;
		int threadNum;

		void start() const{
			int serverFd = socket(AF_INET, SOCK_STREAM, 0);
			if(serverFd < 0){
				std::cerr << "Couldn't create a socket" << std::endl;
				exit(errno);
			}
			sockaddr_in address;
			address.sin_family = AF_INET;
			address.sin_port = htons(port);
			address.sin_addr.s_addr = htonl(INADDR_ANY);

			int bound = bind(serverFd, (const struct sockaddr*) &address, sizeof(address));
			if(bound < 0){
				std::cerr << "Couldn't bind to port" << std::endl;
				exit(errno);
			}

			int listening = listen(serverFd, 1024);
			if(listening < 0){
				std::cerr << "Couldn't start listening" << std::endl;
				exit(errno);
			}

			while(true){
				//?
			}

		}
	};

}


int main(){

	return 0;
}
