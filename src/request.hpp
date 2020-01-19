//
// Created by Andrew Mroczkowski on 1/19/20.
//

#ifndef AMROX_HTTP_REQUEST_HPP
#define AMROX_HTTP_REQUEST_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

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
                const std::vector<uint8_t> &body) :
                request_method_(requestMethod),
                location_(location),
                http_version_(httpVersion),
                headers_(std::make_shared<std::map<std::string, std::string>>(headers)),
                body_(std::make_shared<std::vector<uint8_t>>(body)) {}

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

        // TODO: check if this is working (moving, not copying) as intended
        template<unsigned T>
        Request( RequestBuilderTemplate<T> in ) : Request { in.build() }{ }

    private:

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
                HttpVersion = (1 << 2)
            };
        };

    public:
        RequestBuilderTemplate() {
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::RequestMethod> method(amrox::http_server::RequestMethod method) {
            RequestBuilderTemplate nextBuilder = *this;
            nextBuilder.request_method_ = method;
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::HttpVersion> http_version(const std::string &http_version) {
            RequestBuilderTemplate nextBuilder = *this;
            nextBuilder.http_version_ = std::move(http_version);
            return nextBuilder;
        }

        RequestBuilderTemplate<CurrentSet | BuildBits::Location> location(const std::string &loc) {
            RequestBuilderTemplate nextBuilder = *this;
            nextBuilder.location_ = std::move(loc);
            return nextBuilder;
        }

        // TODO: header and body methods


        // TODO: make this work
//        explicit operator Request&&() {
//            return build();
//        }

        // TODO: move to private?
        Request&& build() {
            constexpr auto required_fields{
                    BuildBits::RequestMethod |
                    BuildBits::HttpVersion |
                    BuildBits::Location
            };
            static_assert((CurrentSet & required_fields) == required_fields);

            return std::move( Request {request_method_, location_, http_version_, headers_, body_} );
        }

    private:
        template<unsigned OtherSet>
        RequestBuilderTemplate(const RequestBuilderTemplate<OtherSet> &cpy) {
            request_method_ = cpy.request_method_;
            location_ = cpy.location_;
            http_version_ = cpy.http_version_;
        }

        amrox::http_server::RequestMethod request_method_;
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
