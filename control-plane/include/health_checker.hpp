#pragma once

#include "node_registry.hpp"

namespace proxy_scheduler
{
    class HealthChecker : public std::enable_shared_from_this<HealthChecker>
    {
        using Executor = boost::asio::any_io_executor;
        using Strand = boost::asio::strand<Executor>;
        using Timer = boost::asio::steady_timer;

    public:
        HealthChecker(
            boost::asio::any_io_executor ex,
            NodeRegistry &registry,
            std::chrono::seconds check_interval,
            std::chrono::seconds heartbeat_timeout);

        void start();
        void stop();

    private:
        void schedule_check();
        void run_check();

        Strand strand_;
        Timer timer_;

        NodeRegistry &registry_;
        std::chrono::seconds check_interval_;
        std::chrono::seconds heartbeat_timeout_;
        bool stopped_{false};
    };
}