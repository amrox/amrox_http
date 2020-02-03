//
// Created by Andrew Mroczkowski on 2/2/20.
//
#pragma once

#include <cstdint>
#include <vector>
#include <string>

#include "amrox_http/request.hpp"

namespace amrox::http_server {

    /*  https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
    *
    * HTTP requests, and responses, share similar structure and are composed of:
    *      1) A start-line describing the requests to be implemented, or its status of whether successful or a failure. This start-line is always a single line.
    *      2) An optional set of HTTP headers specifying the request, or describing the body included in the message.
    *      3) A blank line indicating all meta-information for the request have been sent.
    *      4) An optional body containing data associated with the request (like content of an HTML form), or the document associated with a response. The presence of the body and its size is specified by the start-line and HTTP headers.
    */
    auto parse_request(const std::vector<uint8_t>::const_iterator begin, const std::vector<uint8_t>::const_iterator end) noexcept-> std::optional<Request>;

}


