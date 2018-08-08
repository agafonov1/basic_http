#include "server.hpp"
#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>
//const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;
static const char* templ = "HTTP/1.0 200 OK\r\n"

		           "Content-length: %d\r\n"

		       	   "Connection: close\r\n"

		       	   "Content-Type: text/html\r\n"

		       	   "\r\n"

		       	   "%s";
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
      std::ofstream logger("/tmp/log.d", std::ofstream::app);
      logger<<data;	
      logger.close();	
      //std::cout<<data;
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
		
		char res[200];
		std::string p;
		std::getline(ifs, p);
		sprintf(res, templ, p.size(), p.c_str());
		//std::cout<<res<<std::endl;
		std::string Return(res);
		boost::asio::write(*sock, boost::asio::buffer(Return.c_str(), Return.size()));
		sock.get()->close();///shutdown();
//		static const char *templ = "HTTP/1.0 200 OK\r\nContent-length: %d\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n%s";

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


