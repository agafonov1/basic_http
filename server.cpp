#include "server.hpp"
#include <string>
#include <sstream>
#include <fstream>
//const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
static const char templ[] = "HTTP/1.0 200 OK\r\nContent-length: %d\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n%s";
static const char bad_request[] = "HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n";
void session(socket_ptr sock, std::string path)
{
  try
  {
    for (;;)
    {
      char data[max_length];

      boost::system::error_code error;
      if(!sock->is_open()) return;	
      size_t length = sock->read_some(boost::asio::buffer(data), error);
      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error. 
      std::istringstream request(data);		
      std::string methode;
      request>>methode;	 
      std::string file;		
      if(methode == "GET"){
	//std::cout<<"new get request"<<std::endl;
	request>>file;
	std::string full_path =path+ file;
	//std::cout<<"requested: "<<full_path<<std::endl;
	std::ifstream ifs;
	ifs.open(full_path.c_str());
	if(ifs.good()){
		boost::asio::write(*sock, boost::asio::buffer(templ, sizeof(templ)));
		sock.get()->close();///shutdown();
		//std::cout<<"file exists"<<std::endl;
	}else{
		boost::asio::write(*sock, boost::asio::buffer(bad_request, sizeof(bad_request)));
		sock.get()->close();///shutdown();
	}
      }	
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception in thread: " << e.what() << "\n";
  }
}

void server(boost::asio::io_service& io_context, unsigned short port, std::string filepath, std::string IP_addr)
{
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;)
  {
    socket_ptr sock(new tcp::socket(io_context));
    a.accept(*sock);
    boost::thread t(boost::bind(session, sock, filepath));
  }
}


