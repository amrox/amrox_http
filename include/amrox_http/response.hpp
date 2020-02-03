//
// Created by Andrew Mroczkowski on 2/2/20.
//

#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace amrox::http_server {

    template<unsigned>
    class ResponseBuilderTemplate;

    using ResponseBuilder = ResponseBuilderTemplate<0>;

    class Response {

    public:

        template<unsigned>
        friend class ResponseBuilderTemplate;

        static ResponseBuilder make();

        Response(int status_code, // TODO: valid codes
                 std::string httpVersion,
                 std::multimap<std::string, std::string> headers,
                 std::vector<uint8_t> body)
                : status_code_(status_code),
                  http_version_(std::move(httpVersion)),
                  headers_(std::move(headers)),
                  body_(std::move(body))
        {
        }

        auto status_code() const -> int {
            return status_code_;
        }

        auto http_version() const -> std::string {
            return http_version_;
        }

        auto headers() const -> std::multimap<std::string, std::string> {
            return headers_;
        }

        auto body() const -> std::vector<uint8_t > {
            return body_;
        }

    private:

        Response()=delete;

        int status_code_;
        std::string http_version_;
        std::multimap<std::string, std::string> headers_;
        std::vector<uint8_t> body_;

    };

    template<unsigned CurrentSet>
    class ResponseBuilderTemplate {
        struct BuildBits {
            enum {
                StatusCode = (1 << 0),
                HttpVersion = (1 << 2),
                Headers= (1 << 3),
                Body = (1 << 4),
            };
        };

    public:

        ResponseBuilderTemplate()= default;

        ResponseBuilderTemplate<CurrentSet | BuildBits::StatusCode> status_code(int status_code) {
            ResponseBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.status_code_ = status_code;
            return nextBuilder;
        }

        ResponseBuilderTemplate<CurrentSet | BuildBits::HttpVersion> http_version(const std::string &http_version) {
            ResponseBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.http_version_ = http_version;
            return nextBuilder;
        }

        ResponseBuilderTemplate<CurrentSet | BuildBits::Headers> headers(const std::multimap<std::string, std::string> &headers) {
            ResponseBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.headers_ = headers;
            return nextBuilder;
        }

        ResponseBuilderTemplate<CurrentSet | BuildBits::Body> body(const std::vector<uint8_t > &body) {
            ResponseBuilderTemplate nextBuilder = std::move(*this);
            nextBuilder.body_ = body;
            return nextBuilder;
        }

        // No Copy

        ResponseBuilderTemplate( ResponseBuilderTemplate &) = delete;

        template<unsigned OtherSet>
        ResponseBuilderTemplate & operator= ( const ResponseBuilderTemplate<OtherSet> &other ) = delete;

        operator Response() && {
            return build();
        }

    private:

        template<unsigned OtherSet>
        ResponseBuilderTemplate & operator= ( const ResponseBuilderTemplate<OtherSet> &&other ) {
            status_code_ = other.status_code_;
            http_version_ = std::move(other.http_version_);
            headers_ = std::move(other.headers_);
            body_ = std::move(other.body_);
            return *this;
        }

        template<unsigned OtherSet>
        ResponseBuilderTemplate(const ResponseBuilderTemplate<OtherSet> &&other) noexcept
                : status_code_{},
                  http_version_ {},
                  headers_ {},
                  body_ {}
        {
            *this = std::move(other);
        }

        Response build() {

            constexpr auto required_fields{
                    BuildBits::StatusCode |
                    BuildBits::HttpVersion
            };
            static_assert((CurrentSet & required_fields) == required_fields);

            return Response {
                    status_code_,
                    std::move(http_version_),
                    std::move(headers_),
                    std::move(body_)
            };
        }

        int status_code_;
        std::string http_version_;
        std::multimap<std::string, std::string> headers_;
        std::vector<uint8_t> body_;

        template<unsigned>
        friend class ResponseBuilderTemplate;

        friend class Response;
    };

    inline ResponseBuilder Response::make() {
        return ResponseBuilder();
    }
};

// NOTES:
//   I used the build pattern here as an exercise. In practice, it's a lot of boilerplate. I'm
//   not sure I'd do something similar in production code.
//
//   See 'request.hpp'
