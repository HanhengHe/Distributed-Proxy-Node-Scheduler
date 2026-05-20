#pragma once

#include "type.hpp"

#include <chrono>
#include <string>
#include <cstdint>

namespace proxy_scheduler
{
    struct Node
    {
        NodeId node_id;
        std::string region;
        std::string host;
        std::uint16_t port;

        NodeStatus status{NodeStatus::Healthy};

        std::uint32_t current_load{0};
        std::uint32_t max_capacity{0};
        std::uint32_t latency_ms{0};

        std::chrono::steady_clock::time_point last_heartbeat;
    };
}