//
// Created by Andrew Mroczkowski on 1/19/20.
//

#ifndef AMROX_HTTP_REQUEST_HPP
#define AMROX_HTTP_REQUEST_HPP

#include <cstdint>
#include <memory>
#include <string>
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

        static RequestBuilder make();

        Request(RequestMethod requestMethod,
                const std::string &location,
                const std::string &httpVersion,
                const std::map<std::string, std::string> &headers,
                const std::vector<uint8_t> &body)
                : request_method_(requestMethod),
                  location_(location),
                  http_version_(httpVersion),
                  headers_(std::make_shared<std::map<std::string, std::string>>(headers)),
                  body_(std::make_shared<std::vector<uint8_t>>(body)) {

            // std::cout << "calling regular constructor" << std::endl;
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

        auto headers() const -> std::shared_ptr<std::map<std::string, std::string>> {
            return headers_;
        }

        auto body() const -> std::shared_ptr<std::vector<uint8_t >> {
            return body_;
        }

    private:

        Request();

        RequestMethod request_method_;
        std::string location_;
        std::string http_version_;

        // TODO: do we want shared pointers here?
        std::shared_ptr<std::map<std::string, std::string>> headers_;
        std::shared_ptr<std::vector<uint8_t>> body_;
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
        RequestBuilderTemplate() {
        }

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
            nextBuilder.body_ = body_;
            return nextBuilder;
        }

        operator Request&&() {
            return build();
        }

        // No Copy

       RequestBuilderTemplate( RequestBuilderTemplate &) = delete;

        template<unsigned OtherSet>
        RequestBuilderTemplate  & operator= ( const RequestBuilderTemplate<OtherSet> &other ) = delete;

    private:

        template<unsigned OtherSet>
        RequestBuilderTemplate(const RequestBuilderTemplate<OtherSet> &&other) noexcept
                : request_method_ { other.request_method_ },
                  location_ { other.location_ },
                  http_version_ { other.http_version_ }
        {};

        Request&& build() {
            constexpr auto required_fields{
                    BuildBits::RequestMethod |
                    BuildBits::HttpVersion |
                    BuildBits::Location
            };
            static_assert((CurrentSet & required_fields) == required_fields);

            return std::move( Request {request_method_, location_, http_version_, headers_, body_} );
        }

        RequestMethod request_method_;
        std::string location_;
        std::string http_version_;

        std::map<std::string, std::string> headers_;
        std::vector<uint8_t> body_;

        template<unsigned>
        friend
        class RequestBuilderTemplate;
    };

    RequestBuilder Request::make() {
        return RequestBuilder();
    }
}

#endif //AMROX_HTTP_REQUEST_HPP
