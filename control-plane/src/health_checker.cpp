#include "health_checker.hpp"

namespace proxy_scheduler
{
    HealthChecker::HealthChecker(
        boost::asio::any_io_executor ex,
        NodeRegistry &registry,
        std::chrono::seconds check_interval,
        std::chrono::seconds heartbeat_timeout)
        : strand_(boost::asio::make_strand(ex)),
          timer_(strand_),
          registry_(registry), check_interval_(check_interval),
          heartbeat_timeout_(heartbeat_timeout)
    {
    }

    void HealthChecker::start()
    {
        boost::asio::dispatch(
            strand_,
            [self = shared_from_this()]
            {
                self->stopped_ = false;
                self->schedule_check();
            });
    }

    void HealthChecker::stop()
    {
        boost::asio::dispatch(
            strand_,
            [self = shared_from_this()]
            {
                self->stopped_ = true;
                self->timer_.cancel();
            });
    }

    void HealthChecker::schedule_check()
    {
        if (stopped_)
        {
            return;
        }

        timer_.expires_after(check_interval_);

        timer_.async_wait(boost::asio::bind_executor(
            strand_,
            [self = shared_from_this()](const boost::system::error_code &ec)
            {
                if (ec || self->stopped_)
                {
                    return;
                }

                self->run_check();
                self->schedule_check();
            }));
    }

    void HealthChecker::run_check()
    {
        registry_.mark_expired_nodes(heartbeat_timeout_);
    }
}