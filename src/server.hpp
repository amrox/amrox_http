#pragma once

#include <boost/asio.hpp>

// Largely copied from https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/example/cpp11/echo/async_tcp_echo_server.cpp

namespace amrox::http_server
{

class Session
    : public std::enable_shared_from_this<Session>
{
public:
    Session(boost::asio::ip::tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    if (!ec)
                                    {
                                        do_write(length);
                                    }
                                });
    }

    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
                                 [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                                     if (!ec)
                                     {
                                         do_read();
                                     }
                                 });
    }

    boost::asio::ip::tcp::socket socket_;
    enum
    {
        max_length = 1024
    };
    char data_[max_length];
};

class Server
{

public:
    explicit Server(boost::asio::io_context &io_context, short port)
        : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket))->start();
                }

                do_accept();
            });
    }

    boost::asio::ip::tcp::acceptor acceptor;
};
}; // namespace amrox::http_server