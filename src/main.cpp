#include <iostream>

#include <boost/asio.hpp>
#include <boost/filesystem.hpp>
#include <zmq.h>
#include <docopt.h>
#include <spdlog/spdlog.h>

#include "server.hpp"

namespace bf = boost::filesystem;

static const char USAGE[] =
    R"(amrox's http server

    Usage:
      amrox_http [--port=<port>] [--path=<path>]
      amrox_http (-h | --help)
      amrox_httlp --version

    Options:
      -h --help      Show this screen.
      --version      Show version.
      --path=<path>  Path to server [default: .].
      --port=<port>  Port [default: 7777].
)";

int main(int argc, char **argv)
{
  auto args = docopt::docopt(USAGE,
                             {argv + 1, argv + argc},
                             true,                       // show help if requested
                             "amrox's http server 0.1"); // version string

  bf::path path{args["--path"].asString()};
  if (!bf::is_directory(path))
  {
    spdlog::error("'{}' is not a directory, aborting.", path.string());
    return 1;
  }

  boost::asio::io_context io_context;
  short port = static_cast<short>(args["--port"].asLong());

  spdlog::info("Starting server on port {}", port);

  amrox::http_server::Server s(io_context, port);
  io_context.run();

  return 0;
}
