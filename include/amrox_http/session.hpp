#pragma once

#include <boost/asio.hpp>

namespace amrox::http_server
{

class Session
    : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start();

private:
    void do_read();
    void do_write(std::size_t length);

    boost::asio::ip::tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};

}; // namespace amrox::http_server