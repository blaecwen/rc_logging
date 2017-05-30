#include "discoveryserver.h"


namespace rclog {


DiscoveryServer::DiscoveryServer(boost::asio::io_service &io_service, const params &p)
    : endpoint_(boost::asio::ip::address::from_string(p.multicast_address), p.multicast_port),
      socket_(io_service, endpoint_.protocol()),
      timer_(io_service),
      interval_(p.interval)
{
    hello_message_= std::to_string(p.priority) + ":" + p.node_id;

    // Create the socket so that multiple may be bound to the same address.
    auto listen_address = boost::asio::ip::address::from_string("0.0.0.0");
    boost::asio::ip::udp::endpoint listen_endpoint(listen_address, p.multicast_port);
    socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket_.bind(listen_endpoint);

    // Join the multicast group.
    socket_.set_option(
        boost::asio::ip::multicast::join_group(
            boost::asio::ip::address::from_string(p.multicast_address)));

    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&DiscoveryServer::handle_receive_from, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

    boost::asio::ip::multicast::enable_loopback option(p.enable_loopback);
    socket_.set_option(option);

    socket_.async_send_to(
        boost::asio::buffer(hello_message_), endpoint_,
        boost::bind(&DiscoveryServer::handle_send_to, this,
        boost::asio::placeholders::error));

    std::cerr << "Sending hello messages to '"
              << p.multicast_address << ":" << p.multicast_port
              << "' with '" << p.interval << "' seconds interval"
              << std::endl;
}


void DiscoveryServer::handle_send_to(const boost::system::error_code& error)
{
    if (error)
        std::cerr << "Erorr: error when sending helo message: " << error << std::endl;

    timer_.expires_from_now(boost::posix_time::seconds(interval_));
    timer_.async_wait(
        boost::bind(&DiscoveryServer::handle_timeout, this,
            boost::asio::placeholders::error));
}

void DiscoveryServer::handle_timeout(const boost::system::error_code& error)
{
    if (error)
        std::cerr << "Error: error with timer for hello messages: " << error << std::endl;

    socket_.async_send_to(
        boost::asio::buffer(hello_message_), endpoint_,
        boost::bind(&DiscoveryServer::handle_send_to, this,
          boost::asio::placeholders::error));
}

void DiscoveryServer::handle_receive_from(const boost::system::error_code& error,
    size_t bytes_recvd)
{
    if (error)
        std::cerr << "Error: error when receiving hello message: " << std::endl;
    else {
        std::cout.write(data_, bytes_recvd);
        std::cout << std::endl;
    }

    socket_.async_receive_from(
        boost::asio::buffer(data_, max_length), sender_endpoint_,
        boost::bind(&DiscoveryServer::handle_receive_from, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
}

} // namespace rclog

