#include "localserver.h"


namespace rclog {

LocalServer::LocalServer(boost::asio::io_service &io_service, short port)
    :socket_(io_service, datagram_protocol::endpoint("/tmp/test.socket")) {
    do_receive();
}

void LocalServer::do_receive() {
    socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                               boost::bind(&LocalServer::handle_message, this, _1, _2));
}

void LocalServer::do_send(std::size_t length) {
  std::cout << "EP: " << sender_endpoint_ << std::endl;
  socket_.async_send_to(
      boost::asio::buffer(data_, length), sender_endpoint_,
      [this](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {
        do_receive();
  });
}

void LocalServer::handle_message(boost::system::error_code ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd > 0) {
        std::cout << "Got: '";
        std::cout.write(data_, bytes_recvd);
        std::cout << "'" << std::endl;
        do_send(bytes_recvd);
    }
    else
        do_receive();
}

}
