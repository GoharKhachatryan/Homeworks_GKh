#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>

// function to receive bytes from socket to make request string
std::string receiveRequestAsString(int sFd){
        long bytes = -1;
        // string where we will read bytes
        std::string result;

        // starting to read in the infinite loop
        while(true){

                // creating a buffer to read bytes part by part
                char* buff = new char[500];

                // reading with recv from socket with descriptor sFd
                bytes = recv(sFd, (void *) buff, 500, 0);

                // possible cases

                // error during the reading
                if(bytes < 0){
                        std::cerr << "Something went wrong during reading from socket with FD " << sFd
                                  << "Error code is: " << errno << std::endl;
                        close(sFd);
                        return {};
                }

                // nothing to read
                if(bytes == 0){
                        // break the infinite loop 'cuz reading from socket is over
                        break;
                }

                // the reading was success so we can add the buff to result string
                result = result + buff;
        }
        // after reading everything from socket function will return the result string
        return result;
}


// sent the response string through socket
void sendResponse(int sFd, std::string response){

}




class http{
public:
	// constructor
	http()
		: port(8080)
	{}

	explicit http(int port)
		: port(port)
	{}

	// function to start server
	void start(){
		int serverFd = socket(AF_INET, SOCK_STREAM, 0);
		// if couldn't create a socket
		if(serverFd < 0){
			std::cerr << "Something went wrong while creating a socket" << std::endl;
			exit(errno);
		}

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_port = htons(port);
		address.sin_addr.s_addr = htonl(INADDR_ANY);

		// binding socket
		int bound = bind(serverFd, (const struct sockaddr*) &address, sizeof(address));
		// if couldn't bind to the port
		if(bound < 0){
			std::cerr << "Something went wrong while binding to given port" << std::endl;
			exit(errno);
		}

		// starting listening
		int listening = listen(serverFd, 1024);
		// if couldn't start listening
		if(listening < 0){
			std::cerr << "Something went wrong during the start of listening" << std::endl;
			exit(errno);
		}

		// start a thread pool (?)

		while(true){
			sockaddr_in client_address;
			unsigned int client_address_length;
			int clientFd = accept(serverFd, (struct sockaddr*) &client_address, &client_address_length);

			// thread pool (?)

			Request request;
			Response response;
			std::string req;
			std::string res;

			req = receiveRequestAsString(clientFd);
			request.stringToRequest(req);
			response = response.makeResponse(request);
			res = response.ResponseToString();
			sendResponse(clientFd, res);
		}
	}

	// handling (?)
private:
	int port;

	class Request{
	private:
		std::string method;
		std::string path;
		std::map<std::string, std::string> header;
		std::string body;
		std::string version;
		// Content length, content type, accept type (?)

	public:
                // constructor
                Request()
                        : method{}
                        , path{}
                        , header{}
                        , body{}
                        , version{}
                {}

		// function to make a Request class object from string
                void stringToRequest(std::string &request){
			
		}


	};

	// class Response
	class Response{

	private:
		std::string version;
		std::string statusNumber;
		std::string statusText;
		std::map<std::string, std::string> header;
		std::string body;
		// Content length, content type (?)

	public:
                // constructor
                Response()
                        : version{}
                        , statusNumber{}
                        , statusText{}
                        , header{}
                        , body{}
                {}


		// function to handle (?)
		Response makeResponse(Request request){

		}

		// convert Response object to string
		std::string ResponseToString(){

		}
	};

};


/*// function to receive bytes from socket to make request string
std::string receiveRequestAsString(int sFd){
	long bytes = -1;
        // string where we will read bytes
        std::string result;

	// starting to read in the infinite loop
	while(true){

		// creating a buffer to read bytes part by part
		char* buff = new char[500];

                // reading with recv from socket with descriptor sFd
                bytes = recv(sFd, (void *) buff, 500, 0);

                // possible cases

                // error during the reading
                if(bytes < 0){
			std::cerr << "Something went wrong during reading from socket with FD " << sFd
				  << "Error code is: " << errno << std::endl;
			close(sFd);
                        return {};
		}

                // nothing to read
                if(bytes == 0){
			// break the infinite loop 'cuz reading from socket is over
                        break;
		}

                // the reading was success so we can add the buff to result string
                result = result + buff;
	}
        // after reading everything from socket function will return the result string
        return result;
}


// sent the response string through socket
void sendResponse(int sFd, std::string response){

}
*/

int main(){

	return 0;
}
