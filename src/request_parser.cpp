//
// Created by Andrew Mroczkowski on 2/2/20.
//

#include "amrox_http/request_parser.hpp"

#include <regex>

using namespace amrox::http_server;

/*  https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
*
* HTTP requests, and responses, share similar structure and are composed of:
*      1) A start-line describing the requests to be implemented, or its status of whether successful or a failure. This start-line is always a single line.
*      2) An optional set of HTTP headers specifying the request, or describing the body included in the message.
*      3) A blank line indicating all meta-information for the request have been sent.
*      4) An optional body containing data associated with the request (like content of an HTML form), or the document associated with a response. The presence of the body and its size is specified by the start-line and HTTP headers.
*/

auto amrox::http_server::parse_request(const std::vector<uint8_t>::const_iterator begin, const std::vector<uint8_t>::const_iterator end) noexcept -> std::optional<Request>
{
    // Search for 3) blank line
    static const std::vector<uint8_t> blank_line {'\r', '\n', '\r', '\n' };
    const auto&& meta_end = std::search(begin, end, blank_line.cbegin(), blank_line.cend());
    if (meta_end == end) {
        return std::nullopt;
    }

    // Now parse 1) start-line
    static const std::vector<uint8_t> crlf {'\r', '\n'};
    static const std::regex start_line_regex {"([A-Z].*) (.*) ([A-Z].*\\/\\d.*)" } ;
    const auto&& start_line_end = std::search(begin, end, crlf.cbegin(), crlf.cend());
    std::string start_line { begin, start_line_end };
    std::smatch start_line_matches;
    std::regex_match(start_line, start_line_matches, start_line_regex);

    // The first sub_match is the whole string; the next
    // sub_match is the first parenthesized expression.
    if (start_line_matches.size() != 4) {
        return std::nullopt;
    }

    // Parse request method from start line

    static const std::map<std::string, RequestMethod> string_to_method {
            { "GET", RequestMethod::GET }
    };

    const auto&& request_method_pos = string_to_method.find(start_line_matches[1].str());
    if (request_method_pos == string_to_method.end()) {
        return std::nullopt;
    }
    const RequestMethod request_method = request_method_pos->second;

    // Parse location from start line

    const std::string location = start_line_matches[2].str();

    // TODO: validate location

    // Parse http version from start line

    const std::string http_version = start_line_matches[3].str();
    if (http_version != "HTTP/1.1") {
        // only supporting HTTP 1.1 for now
        return std::nullopt;
    }

    // Parse headers
    static const std::regex header_line_regex {"([A-Za-z\\-]+):\\s*(.*)" };
    std::string raw_headers { start_line_end + crlf.size(), meta_end };
    const auto&& headers_begin = std::sregex_iterator(raw_headers.cbegin(), raw_headers.end(), header_line_regex);
    const auto&& headers_end = std::sregex_iterator();

    std::multimap<std::string, std::string> headers;

    for (std::sregex_iterator i = headers_begin; i != headers_end; ++i) {
        std::smatch match = *i;
        headers.insert({ match[1].str(), match[2].str() });
    }

    return Request {
            request_method,
            location,
            http_version,
            headers,
            {}}; // TODO: (later) body parsing
}

