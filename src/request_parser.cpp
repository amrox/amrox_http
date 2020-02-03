//
// Created by Andrew Mroczkowski on 2/2/20.
//

#include "amrox_http/request_parser.hpp"

using namespace amrox::http_server;

auto amrox::http_server::parse_request(const std::vector<uint8_t>::const_iterator begin, const std::vector<uint8_t>::const_iterator end) noexcept -> std::optional<Request>
{
    // Search for 3) blank line
    static const std::vector<uint8_t> blank_line {'\r', '\n', '\r', '\n' };
    const auto meta_end = std::search(begin, end, blank_line.cbegin(), blank_line.cend());
    if (meta_end == end) {
        return std::nullopt;
    }

    // Now parse 1) start-line
    static const std::vector<uint8_t> crlf {'\r', '\n'};
    static const std::regex start_line_regex {"([A-Z].*) (.*) ([A-Z].*\\/\\d.*)" } ;
    auto const start_line_end = std::search(begin, end, crlf.cbegin(), crlf.cend());
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

    auto const request_method_pos = string_to_method.find(start_line_matches[1].str());
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
    auto const headers_begin = std::sregex_iterator(raw_headers.cbegin(), raw_headers.end(), header_line_regex);
    auto const headers_end = std::sregex_iterator();

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


