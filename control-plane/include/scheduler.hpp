#pragma once
#include "node.hpp"
#include "node_registry.hpp"

#include <boost/asio.hpp>

namespace proxy_scheduler
{
    struct SchedulingRequest
    {
        std::optional<std::string> preferred_region;
        std::optional<std::uint32_t> max_latency_ms;

        bool allow_cross_region{true};
    };

    struct SchedulingDecision
    {
        Node node;
        double score{0.0};
    };

    class Scheduler
    {
    public:
        Scheduler(NodeRegistry &registry);

        void select_node(
            SchedulingRequest &&request,
            std::function<void(std::optional<SchedulingDecision>)> callback) const;

    private:
        static bool is_eligible(
            const Node &node,
            const SchedulingRequest &request);

        static double calculate_score(
            const Node &node,
            const SchedulingRequest &request);

    private:
        NodeRegistry &registry_;
    };
}