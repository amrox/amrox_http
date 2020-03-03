#include "amrox_http/server.hpp"
#include "amrox_http/session.hpp"

using amrox::http_server::Server;
using amrox::http_server::Session;

Server::Server(boost::asio::io_context &io_context, short port)
    : acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{

    do_accept();
}

void Server::do_accept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
            if (!ec)
            {
                std::make_shared<Session>(std::move(socket))->start();
            }

            do_accept();
        });
}
