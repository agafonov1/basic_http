#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>

#pragma once

using boost::asio::ip::tcp;

const int max_length = 1024;

typedef boost::shared_ptr<tcp::socket> socket_ptr;

void server(boost::asio::io_service& io_context, unsigned short port, std::string filepath, std::string IP_addr);
void session(socket_ptr sock, std::string path);

