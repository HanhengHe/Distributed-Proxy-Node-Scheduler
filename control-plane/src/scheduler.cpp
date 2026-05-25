#include "scheduler.hpp"

namespace proxy_scheduler
{
    Scheduler::Scheduler(NodeRegistry &registry)
        : registry_(registry)
    {
    }

    void Scheduler::select_node(
        SchedulingRequest &&request,
        std::function<void(std::optional<SchedulingDecision>)> callback) const
    {
        registry_.get_healthy_nodes(
            [request = std::move(request), callback = std::move(callback)](std::vector<Node> nodes)
            {
                std::optional<SchedulingDecision> best_decision;

                for (const auto &node : nodes)
                {
                    if (!is_eligible(node, request))
                    {
                        continue;
                    }

                    const double score = calculate_score(node, request);
                    if (!best_decision || score > best_decision->score)
                    {
                        best_decision = SchedulingDecision{node, score};
                    }
                }
                callback(best_decision);
            });
    }

    bool Scheduler::is_eligible(
        const Node &node,
        const SchedulingRequest &request)
    {
        if (node.status != NodeStatus::Healthy)
        {
            return false;
        }

        if (node.max_capacity == 0)
        {
            return false;
        }

        if (node.current_load >= node.max_capacity)
        {
            return false;
        }

        if (request.max_latency_ms &&
            node.latency_ms > *request.max_latency_ms)
        {
            return false;
        }

        if (request.preferred_region &&
            !request.allow_cross_region &&
            node.region != *request.preferred_region)
        {
            return false;
        }

        return true;
    }

    double Scheduler::calculate_score(
        const Node &node,
        const SchedulingRequest &request)
    {
        double score = 0.0;

        // Latency
        score += 1000.0 / (1.0 + static_cast<double>(node.latency_ms));

        // Load
        const double load_ratio =
            static_cast<double>(node.current_load) /
            static_cast<double>(node.max_capacity);
        score += (1.0 - load_ratio) * 100.0;

        // Preferred region
        if (request.preferred_region &&
            node.region == *request.preferred_region)
        {
            score += 100.0;
        }

        return score;
    }
}