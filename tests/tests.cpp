//
// Created by Andrew Mroczkowski on 1/18/20.
//

#include <gtest/gtest.h>

#include <amrox_http/request.hpp>
#include <amrox_http/response.hpp>
#include <amrox_http/request_parser.hpp>


using amrox::http_server::Request;
using amrox::http_server::RequestMethod;
using amrox::http_server::RequestBuilder;
using amrox::http_server::Response;
using amrox::http_server::ResponseBuilder;


TEST(RequestTests, BuildRequest) {

    const RequestMethod request_method { RequestMethod::GET };
    const std::string http_version { "HTTP/1.1" };
    const std::string location { "/ "};
    const std::multimap<std::string, std::string> headers
            { {"host",  "mrox.co"} };

    Request r { request_method, location, http_version, headers, {} };

    EXPECT_EQ(r.method(), request_method);
    EXPECT_EQ(r.location(), location);
    EXPECT_EQ(r.http_version(), http_version);
    EXPECT_EQ(r.headers(), headers);
    EXPECT_TRUE((r.body().empty()));
    EXPECT_EQ(r.headers(), headers);
    EXPECT_TRUE((r.body().empty()));
}

TEST(RequestTests, Builder) {

    const std::multimap<std::string, std::string> headers
            { {"host",  "mrox.co"} };
    const std::vector<uint8_t> body { '1' };

    Request req1 = RequestBuilder()
            .method(RequestMethod::GET)
            .http_version("HTTP/1.1")
            .location("/")
            .headers(headers);

    Request req2 = Request::make()
            .method(RequestMethod::GET)
            .http_version("HTTP/1.1")
            .location("/")
            .body(body);

    EXPECT_EQ(req1.method(), RequestMethod::GET);
    EXPECT_EQ(req2.method(), RequestMethod::GET);
    EXPECT_EQ(req1.method(), req2.method());

    EXPECT_EQ(req1.location(), "/");
    EXPECT_EQ(req2.location(), "/");
    EXPECT_EQ(req1.location(), req2.location());

    EXPECT_EQ(req1.http_version(), "HTTP/1.1");
    EXPECT_EQ(req2.http_version(), "HTTP/1.1");
    EXPECT_EQ(req1.http_version(), req2.http_version());

    EXPECT_EQ(req1.headers(), headers);
    EXPECT_TRUE(req1.body().empty());

    EXPECT_TRUE(req2.headers().empty());
    EXPECT_EQ(req2.body(), body);
}


TEST(RequestTests, Parse1) {

    std::string s1 { "GET / HTTP/1.1\r\nHost: localhost:3000\r\nUser-Agent: amrox_http_tests\r\n\r\n" };
    std::vector<uint8_t> b1 { s1.begin(), s1.end() };

    auto r = amrox::http_server::parse_request(b1.begin(), b1.end());

    EXPECT_EQ(r->method(), RequestMethod::GET);
    EXPECT_EQ(r->location(), "/");
    EXPECT_EQ(r->http_version(), "HTTP/1.1");
    EXPECT_EQ(r->headers().find("Host")->second, "localhost:3000");
    EXPECT_EQ(r->headers().find("User-Agent")->second, "amrox_http_tests");

    // TODO: case-insentitve header support
}

TEST(RequestTests, Parse_Bad_Method) {

    std::string s1 { "MERP / HTTP/1.1\r\n\r\n" };
    std::vector<uint8_t> b1 { s1.begin(), s1.end() };

    auto r = amrox::http_server::parse_request(b1.begin(), b1.end());

    EXPECT_FALSE(r);
}

TEST(RequestTests, Parse_Bad_HttpVersion) {

    std::string s1 { "GET /thing HTTP/1.0\r\n\r\n" };
    std::vector<uint8_t> b1 { s1.begin(), s1.end() };

    auto r = amrox::http_server::parse_request(b1.begin(), b1.end());

    EXPECT_FALSE(r);
}

TEST(ResponseTests, BuildResponse) {

    const int status_code { 200 };
    const std::string http_version { "HTTP/1.1" };
    const std::multimap<std::string, std::string> headers
            { {"host",  "mrox.co"} };

    Response r { status_code, http_version, headers, {} };

    EXPECT_EQ(r.status_code(), status_code);
    EXPECT_EQ(r.http_version(), http_version);
    EXPECT_EQ(r.headers(), headers);
    EXPECT_TRUE((r.body().empty()));
    EXPECT_EQ(r.headers(), headers);
    EXPECT_TRUE((r.body().empty()));
}


TEST(ResponseTests, Builder) {

    const std::multimap<std::string, std::string> headers
            { {"host",  "mrox.co"} };
    const std::vector<uint8_t> body { '1' };

    Response req1 = ResponseBuilder()
            .status_code(200)
            .http_version("HTTP/1.1")
            .headers(headers);

    Response req2 = Response::make()
            .status_code(200)
            .http_version("HTTP/1.1")
            .body(body);

    EXPECT_EQ(req1.status_code(), 200);
    EXPECT_EQ(req2.status_code(), 200);
    EXPECT_EQ(req1.status_code(), req2.status_code());

    EXPECT_EQ(req1.http_version(), "HTTP/1.1");
    EXPECT_EQ(req2.http_version(), "HTTP/1.1");
    EXPECT_EQ(req1.http_version(), req2.http_version());

    EXPECT_EQ(req1.headers(), headers);
    EXPECT_TRUE(req1.body().empty());

    EXPECT_TRUE(req2.headers().empty());
    EXPECT_EQ(req2.body(), body);
}
