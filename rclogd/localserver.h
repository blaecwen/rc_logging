#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


namespace rclog {

using boost::asio::ip::udp;


class LocalServer {
public:
    LocalServer(boost::asio::io_service& io_service, short port);

    void do_receive();
    void do_send(std::size_t length);

private:
    void handle_message(boost::system::error_code ec, std::size_t bytes_recvd);

    udp::socket socket_;
    udp::endpoint sender_endpoint_;

    enum { max_length = 1024 };
    char data_[max_length];     // TODO: verify if exceed.
};

}

#endif // LOCALSERVER_H
