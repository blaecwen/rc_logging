#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::local::datagram_protocol;

enum { max_length = 1024 };

const char *client_socket_basename = "/tmp/rclogger.";

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: rclogger <socket_path>\n";
      return 1;
    }

    boost::asio::io_service io_service;

    datagram_protocol::endpoint endpoint(argv[1]);
    datagram_protocol::socket s(io_service);
    s.connect(endpoint);

    std::string client_socket_name = client_socket_basename + std::to_string(::getpid());
    ::unlink(client_socket_name.c_str());     // NOTE: is it correct behavior?
    datagram_protocol::endpoint client_endpoint(client_socket_name);
    s.bind(client_endpoint);

//    std::cout << "Enter message: ";
    char request[max_length];
    std::cin.getline(request, max_length);
    size_t request_length = std::strlen(request);
    s.send_to(boost::asio::buffer(request, request_length), endpoint);

    char reply[max_length];
    size_t reply_length = s.receive_from(
        boost::asio::buffer(reply, max_length), client_endpoint);
    std::cout << "Reply is: ";
    std::cout.write(reply, reply_length);
    std::cout << "\n";

    s.close();
    ::unlink(client_socket_name.c_str());
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
