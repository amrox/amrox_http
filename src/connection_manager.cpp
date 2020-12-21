#include "amrox_http/connection_manager.hpp"

using namespace amrox::http_server;

ConnectionManager::ConnectionManager() 
{
}

void ConnectionManager::start(ConnectionPtr c)
{
    connections_.insert(c);
    c->start();
}

void ConnectionManager::stop(ConnectionPtr c)
{
    c->stop();
    connections_.erase(c);
}