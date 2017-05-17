#include "localserver.h"

// TODO: specify it in cmd args
#define NODE_ID 1

namespace rclog {

LocalServer::LocalServer(boost::asio::io_service &io_service, std::string socket_path, DatabaseManager &dbManager)
    :socket_(io_service, datagram_protocol::endpoint(socket_path)),
     dbManager(dbManager)
{
    do_receive();
}

void LocalServer::do_receive() {
    socket_.async_receive_from(boost::asio::buffer(data_, max_length), sender_endpoint_,
                               boost::bind(&LocalServer::handle_message, this,
                                           boost::asio::placeholders::error,
                                           boost::asio::placeholders::bytes_transferred));
}

void LocalServer::do_send(const std::string &message) {
    // make shared ptr in order to save string after end of the current function
    boost::shared_ptr<std::string> msg = boost::make_shared<std::string>(message);

    socket_.async_send_to(
      boost::asio::buffer(*msg), sender_endpoint_,
      [this, msg](boost::system::error_code /*ec*/, std::size_t /*bytes_sent*/) {});
}

void LocalServer::handle_message(const boost::system::error_code& ec, std::size_t bytes_recvd)
{
    if (!ec && bytes_recvd > 0) {
        std::cout << "Got: '";
        std::cout.write(data_, bytes_recvd);
        std::cout << "'" << std::endl;

        dbManager.addDocument(std::string(data_, bytes_recvd) , "producer");
        do_send("ok");
    }
    else if (ec == boost::asio::error::message_size) {       // NOTE: does not work for some reason
        std::cerr << "Error: got message greater then buffer size" << std::endl;
        do_send("packet_size_error");
    }
    else if (ec) {
        std::cerr << "Error: <" << ec << "> - some problems with receiving message from local socket" << std::endl;
        do_send("error");
    }

    do_receive();
}

}
