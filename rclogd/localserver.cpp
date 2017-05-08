#include "localserver.h"

#include <iostream>

namespace rclog {

LocalServer::LocalServer(boost::asio::io_service &io_service, short port)
    :socket_(io_service, udp::endpoint(udp::v4(), port)) {
    do_receive();
}

void LocalServer::do_receive() {
    socket_.async_receive_from(
      boost::asio::buffer(data_, max_length),
      sender_endpoint_,
      [this](boost::system::error_code ec, std::size_t bytes_recvd) {
      handle_message(ec, bytes_recvd);
    });
    std::cout << "do_receive end" << std::endl;
}

void LocalServer::do_send(std::size_t length) {
  socket_.async_send_to(
      boost::asio::buffer(data_, length), sender_endpoint_,
      [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
        do_receive();
  });
}

void LocalServer::handle_message(boost::system::error_code ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd > 0) {
        do_send(bytes_recvd);
    }
    else
        do_receive();
}

}
