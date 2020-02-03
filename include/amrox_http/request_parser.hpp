//
// Created by Andrew Mroczkowski on 2/2/20.
//
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "amrox_http/request.hpp"

namespace amrox::http_server {

    auto parse_request(const std::vector<uint8_t>::const_iterator begin, const std::vector<uint8_t>::const_iterator end) noexcept -> std::optional<Request>;

}


