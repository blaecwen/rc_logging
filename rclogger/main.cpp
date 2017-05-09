#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::local::datagram_protocol;

enum { max_length = 4096 };

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
    std::string request_str = "";
    std::string line;
    while (std::getline(std::cin, line))
        request_str += line + "\n";

    if (request_str.length() > max_length)
        std::cerr << "Error: message to send is greater then max size!" << std::endl;
    s.send_to(boost::asio::buffer(request_str), endpoint);

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
