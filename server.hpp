#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#pragma once
using boost::asio::ip::tcp;
class session{
private:
	tcp::socket socket_;
	enum{ max_lenght = 1024};
	char data_[max_lenght];
public: 
	session(boost::asio::io_service &io_service):socket_(io_service){}
	tcp::socket &socket() {return socket_;}
	void start();
	void handle_read(const boost::system::error_code &error,size_t bytes_transferred);
	void handle_write(const boost::system::error_code &error);
};
class server{
private:
	boost::asio::io_service &io_service_;
	tcp::acceptor acceptor_;
public:
	server(boost::asio::io_service &io_service,unsigned short port): io_service_(io_service),
					acceptor_(io_service, tcp::endpoint(tcp::v4(), port)){
	session *new_session = new session(io_service_);
	acceptor_.async_accept(new_session->socket(), 
		boost::bind(&server::handle_accept, this,
		new_session,
		boost::asio::placeholders::error));
	}
	void handle_accept(session *new_session,const boost::system::error_code &error);

};
