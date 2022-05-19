#include <iostream>
#include <map>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include <arpa/inet.h>


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

	}

	// handling (?)
private:
	int port;

	class Request{
	public:
		// constructor
		Request()
			: method{}
			, path{}
			, header{}
			, body{}
			, version{}
		{}
	private:
		std::string method;
		std::string path;
		std::map<std::string, std::string> header;
		std::string body;
		std::string version;
		// Content length, content type, accept type (?)

		// function to receive bytes from socket to make request string (DONE)
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


		// function to make a Request class object from string
                Request stringToRequest(long bytes, std::string &request){
			// function split_string
		}


	};

	// class Response
	class Response{
	public:
		// constructor
		Response()
			: version{}
			, statusNumber{}
			, statusText{}
			, header{}
			, body{}
		{}
	private:
		std::string version;
		std::string statusNumber;
		std::string statusText;
		std::map<std::string, std::string> header;
		std::string body;
		// Content length, content type (?)

		// function to handle (?)
		Response makeResponse(Request request){

		}

		// convert Response object to string
		std::string ResponseToString(Response response){

		}

		// sent the ready string through socket
		void sendResponse(int sFd, std::string response){
			return;
		}
	};

};


int main(){

	return 0;
}
