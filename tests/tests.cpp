//
// Created by Andrew Mroczkowski on 1/18/20.
//

#include <gtest/gtest.h>

#include "request.hpp"

using amrox::http_server::Request;
using amrox::http_server::RequestMethod;
using amrox::http_server::RequestBuilder;


TEST(RequestTests, BuildRequest) {

    const RequestMethod request_method { RequestMethod::GET };
    const std::string http_version { "HTTP/1.1" };
    const std::string location { "/ "};
    const std::map<std::string, std::string> headers
            { {"host",  "mrox.co"} };

    Request r { request_method, location, http_version, headers, {} };

    EXPECT_EQ(r.method(), request_method);
    EXPECT_EQ(r.location(), location);
    EXPECT_EQ(r.http_version(), http_version);
    EXPECT_EQ(*r.headers().get(), headers);
    EXPECT_TRUE((r.body()->empty()));
}

TEST(RequestTests, Builder) {

    Request req1 = RequestBuilder()
            .method(RequestMethod::GET)
            .location("/")
            .http_version("HTTP/1.1");

    Request req2 = Request::make()
            .method(RequestMethod::GET)
            .http_version("HTTP/1.1")
            .location("/");

    EXPECT_EQ(req1.method(), RequestMethod::GET);
    EXPECT_EQ(req2.method(), RequestMethod::GET);
    EXPECT_EQ(req1.method(), req2.method());

    EXPECT_EQ(req1.location(), "/");
    EXPECT_EQ(req2.location(), "/");
    EXPECT_EQ(req1.location(), req2.location());

    EXPECT_EQ(req1.http_version(), "HTTP/1.1");
    EXPECT_EQ(req2.http_version(), "HTTP/1.1");
    EXPECT_EQ(req1.http_version(), req2.http_version());

}
