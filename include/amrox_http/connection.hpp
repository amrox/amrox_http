#pragma once

#include <memory>
#include <boost/asio.hpp>

#include "amrox_http/request_handler.hpp"

namespace amrox::http_server {

class Connection
    : std::enable_shared_from_this<Connection>
    /// Represents a single connection from a client.
{
public:
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

  /// Construct a connection with the given socket.
  explicit Connection(boost::asio::ip::tcp::socket socket,
        RequestHandler& handler);

  /// Start the first asynchronous operation for the connection.
  void start();

  /// Stop all asynchronous operations associated with the connection.
  void stop();

};

typedef std::shared_ptr<Connection> ConnectionPtr;

} // namespace amrox::http_server
