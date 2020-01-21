//
// Created by Andrew Mroczkowski on 1/19/20.
//


#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <iostream>

namespace amrox::http_server {

    template<unsigned>
    class RequestBuilderTemplate;

    using RequestBuilder = RequestBuilderTemplate<0>;

    enum class RequestMethod {
        GET,
        // TODO: POST, DELETE, PUT, PATCH
    };

    class Request {

    public:

        template<unsigned>
        friend class RequestBuilderTemplate;

        static RequestBuilder make();

//        Request(RequestMethod requestMethod,
//                const std::string &location,
//                const std::string &httpVersion,
//                const std::map<std::string, std::string> &headers,
//                const std::vector<uint8_t> &body)
//                : request_method_(requestMethod),
//                  location_(location),
//                  http_version_(httpVersion),
//                  headers_(headers),
//                  body_(body)
//        {
////             std::cout << "calling regular constructor" << std::endl;
//        }

        Request(RequestMethod requestMethod,
                std::string location,
                std::string httpVersion,
                std::map<std::string, std::string> headers,
                std::vector<uint8_t> body)
                : request_method_(requestMethod),
                  location_(std::move(location)),
                  http_version_(std::move(httpVersion)),
                  headers_(std::move(headers)),
                  body_(std::move(body))
        {
             std::cout << "calling regular constructor" << std::endl;
        }


        auto method() const -> RequestMethod {
            return request_method_;
        }

        auto location() const -> std::string {
            return location_;
        }

        auto http_version() const -> std::string {
            return http_version_;
        }

        auto headers() const -> std::map<std::string, std::string> {
            return headers_;
        }

        auto body() const -> std::vector<uint8_t > {
            return body_;
        }

//        template<unsigned T>
//        Request(RequestBuilderTemplate<T> &&t)
//                : request_method_ {},
//                  location_ {},
//                  http_version_ {},
//                  headers_ {},
//                  body_ {}
//        {
//            std::cout << "hurk" << std::endl;
//            *this = t.build();
//        }


//        Request& operator= ( const Request&& other )
//        {
//            std::cout << "durk" << std::endl;
//            if (this != &other)
//            {
//                request_method_ = other.request_method_;
//                location_ = std::move(other.location_);
//                http_version_ = std::move(other.http_version_);
//                headers_ = std::move(other.headers_);
//                body_ = std::move(other.body_);
//            }
//            return *this;
//        }


    private:

        Request()=delete;

        RequestMethod request_method_;
        std::string location_;
        std::string http_version_;
        std::map<std::string, std::string> headers_;
        std::vector<uint8_t> body_;

        // TODO: do we want shared pointers here?
        //std::shared_ptr<std::map<std::string, std::string>> headers_;
        //std::shared_ptr<std::vector<uint8_t>> body_;
    };

    template<unsigned CurrentSet>
    class RequestBuilderTemplate {
        struct BuildBits {
            enum {
                RequestMethod = (1 << 0),
                Location = (1 << 1),
                HttpVersion = (1 << 2),
                Headers= (1 << 3),
                Body = (1 << 4),
            };
        };

    public:

        RequestBuilderTemplate()= default;

        RequestBuilderTemplate<CurrentSet | BuildBits::RequestMethod> method(amrox::http_server::RequestMethod method) {
            RequestBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.request_method_ = method;
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::HttpVersion> http_version(const std::string &http_version) {
            RequestBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.http_version_ = http_version;
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::Location> location(const std::string &loc) {
            RequestBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.location_ = loc;
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::Headers> headers(const std::map<std::string, std::string> &headers) {
            RequestBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.headers_ = headers;
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::Body> body(const std::vector<uint8_t > &body) {
            RequestBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.body_ = body;
            return nextBuilder;
        }

        // No Copy

        RequestBuilderTemplate( RequestBuilderTemplate &) = delete;

        template<unsigned OtherSet>
        RequestBuilderTemplate & operator= ( const RequestBuilderTemplate<OtherSet> &other ) = delete;

        operator Request() && {
            return build();
        }

    private:

        template<unsigned OtherSet>
        RequestBuilderTemplate & operator= ( const RequestBuilderTemplate<OtherSet> &&other ) {
            request_method_ = other.request_method_;
            location_ = std::move(other.location_);
            http_version_ = std::move(other.http_version_);
            headers_ = std::move(other.headers_);
            body_ = std::move(other.body_);
            return *this;
        }

        template<unsigned OtherSet>
        RequestBuilderTemplate(const RequestBuilderTemplate<OtherSet> &&other) noexcept
                : request_method_ {},
                  location_ {},
                  http_version_ {},
                  headers_ {},
                  body_ {}
        {
            *this = std::move(other);
        }

        Request build() {

            constexpr auto required_fields{
                    BuildBits::RequestMethod |
                    BuildBits::HttpVersion |
                    BuildBits::Location
            };
            static_assert((CurrentSet & required_fields) == required_fields);

            return Request {
                    request_method_,
                    std::move(location_),
                    std::move(http_version_),
                    std::move(headers_),
                    std::move(body_)
            };
       }

        RequestMethod request_method_;
        std::string location_;
        std::string http_version_;
        std::map<std::string, std::string> headers_;
        std::vector<uint8_t> body_;

        template<unsigned>
        friend class RequestBuilderTemplate;

        friend class Request;
    };

    RequestBuilder Request::make() {
        return RequestBuilder();
    }


    // -----------------------------------------


    auto parse(std::vector<char>::iterator begin, std::vector<char>::iterator end) -> std::optional<Request> {

        const uint8_t CRLF[] { '\r', '\n' };

//        std::string_view sv { begin, end };
////        std::string_view sv { "hello" };
//
////        std::find(begin, end, CRLF);
//        std::find(begin, end, '\r');

        auto cur { begin };
        while (cur != end) {
            if (*cur == '\r') {
                cur++;
                if (*cur != '\n') {
                    std::cerr << "got CR without LF!";
                    return std::nullopt;
                }

                std::string line1 { begin, cur };
                std::cout << line1;



//                if (cur == begin) {
//                    std::cerr << "found \\n at begining!";
//                    return std::nullopt;
//                }
//
//                if ((cur) != '\r') {
//                    std::cerr << "got line feed without CR!";
//                    return std::nullopt;
//                }

            }

            cur++;
        }

        return std::nullopt;
    }
}

