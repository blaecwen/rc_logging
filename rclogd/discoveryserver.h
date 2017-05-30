#ifndef DISCOVERYSERVER_H
#define DISCOVERYSERVER_H

#include <iostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

namespace rclog {



class DiscoveryServer
{
public:
    struct params {
        std::string multicast_address;
        short multicast_port;

        std::string node_id;
        int priority;

        int interval;                    // interval between hello messages
        bool enable_loopback;
    };

    DiscoveryServer(boost::asio::io_service& io_service, const params &p);

    void handle_send_to(const boost::system::error_code &error);
    void handle_timeout(const boost::system::error_code &error);
    void handle_receive_from(const boost::system::error_code &error, size_t bytes_recvd);

private:
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::endpoint sender_endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::deadline_timer timer_;

    int interval_;
    std::string hello_message_;

    enum { max_length = 1024 };
    char data_[max_length];
};


} // namespace rclog

#endif // DISCOVERYSERVER_H
