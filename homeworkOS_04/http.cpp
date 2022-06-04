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
	long bytes = -1;
	// cast to char*
	char* responseChar = const_cast<char*>(response.c_str());

	// send bytes through socket
	bytes = send(sFd, (void*) responseChar, response.size(), 0);

	// error during sending bytes
	if(bytes < 0){
		std::cerr << "Something went wrong during sending bytes through socket" << std::endl;
		close(sFd);
		exit(errno);
	}
	close(sFd);
	return;
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


private:
	int port;

	// map to save handlers for each type of method/path (?)
	//std::map<std::pair<std::string, std::string>, Handler> handler;


	class Request{
	private:
		std::string method;
		std::string path;
		std::string version;
		std::map<std::string, std::string> header;
		std::string body;

	public:
                // constructor
                Request()
                        : method{}
                        , path{}
			, version{}
                        , header{}
                        , body{}
                {}

			
		// function to make a Request class object from string
                void stringToRequest(std::string &request){
                        // part before headers: (before '\n')
                        size_t firstPartEnd = request.find("\n");
                        std::string firstPart = request.substr(0, firstPartEnd);
                        // erase the first part + \n
                        request.erase(0, firstPartEnd + 2);
                        // split FirstPart with " "
                        size_t pos = firstPart.find(" ");
                        method = firstPart.substr(0, pos);
                        // erase method and " "
                        firstPart.erase(0, pos);
                        // repeat for path and version
                        pos = firstPart.find(" ");
                        path = firstPart.substr(0, pos);
                        firstPart.erase(0, pos);
                        pos = firstPart.find(" ");
                        version = firstPart.substr(0, pos);
                        // done with the part before headers

                        // dealing with headers
                        size_t headerEnd = request.find("\n");

                        // second part is the headers with \n
                        std::string secondPart = request.substr(0, headerEnd + 2);
                        request.erase(0, headerEnd + 2);
                        while(secondPart.empty() == false){
                                // find key in string with headers
                                size_t dots = secondPart.find(": ");
                                std::string key = secondPart.substr(0, dots);
                                secondPart.erase(0, dots + 2);
                                // find value in string
                                size_t newstr = secondPart.find("\n");
                                std::string value = secondPart.substr(0, newstr);
                                secondPart.erase(0, newstr + 2);
                                // add to map
                                header[key] = value;
                        }

                        // everything that was left in request string is body
                        body = request;


                        return;
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
			std::string res;

			// the starting line
			res += version + ' ';
			res += statusNumber + ' ';
			res += statusText + '\n';

			// headers by form key: value \n
			for(auto x : header){
				res += x.first + ':' + ' ' + x.second + '\n';
			}

			// if body is empty return the answer string
			if(body.empty() == true){
				return res;
			}

			// adding body and return the answer string
			res += '\n' + body;
			return res;
		}
	};

};


int main(){

	return 0;
}
