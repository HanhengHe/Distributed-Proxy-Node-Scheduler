#include "config.hpp"
#include "health_checker.hpp"
#include "http_server.hpp"
#include "node_registry.hpp"
#include "scheduler.hpp"

#include <boost/asio.hpp>

#include <memory>
#include <iostream>

int main()
{
    try
    {
        boost::asio::io_context io;

        auto registry = std::make_shared<proxy_scheduler::NodeRegistry>(io.get_executor());

        proxy_scheduler::Scheduler scheduler(*registry);

        auto health_checker = std::make_shared<proxy_scheduler::HealthChecker>(
            io.get_executor(),
            *registry,
            std::chrono::seconds(proxy_scheduler::config::heartbeat_interval),
            std::chrono::seconds(proxy_scheduler::config::heartbeat_timeout));

        auto server = std::make_shared<proxy_scheduler::HttpServer>(
            io,
            *registry,
            scheduler,
            proxy_scheduler::config::control_plane_port);

        server->start();
        health_checker->start();

        io.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}