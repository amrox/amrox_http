#pragma once

#include <boost/asio.hpp>

#include "amrox_http/connection_manager.hpp"
#include "amrox_http/request.hpp"

// Largely copied from https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp

namespace amrox::http_server
{

class Server
{

public:
    explicit Server(boost::asio::io_context &io_context, short port);

private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor_;
    ConnectionManager connection_manager_;

};
}; // namespace amrox::http_server