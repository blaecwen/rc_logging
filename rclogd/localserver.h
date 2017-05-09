#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>

namespace rclog {

using boost::asio::local::datagram_protocol;

class LocalServer {
public:
    LocalServer(boost::asio::io_service& io_service, std::string socket_path);

    void do_receive();
    void do_send(const std::string &message);

private:
    void handle_message(boost::system::error_code ec, std::size_t bytes_recvd);

    datagram_protocol::socket socket_;
    datagram_protocol::endpoint sender_endpoint_;

    enum { max_length = 2048 };
    char data_[max_length];     // TODO: verify if exceed.
};

}

#endif // LOCALSERVER_H
