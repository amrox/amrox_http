#pragma once

#include <set>

#include "amrox_http/connection.hpp"

namespace amrox::http_server {

class ConnectionManager {

public:
  /// Construct a connection manager.
  ConnectionManager();

  /// Add the specified connection to the manager and start it.
  void start(ConnectionPtr c);

  /// Stop the specified connection.
  void stop(ConnectionPtr c);

  /// Stop all connections.
  void stop_all();

private:
    std::set<ConnectionPtr> connections_;
};

}
