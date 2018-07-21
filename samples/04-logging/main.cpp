
#include <pride/pride.hpp>

int main()
{
    using namespace pride::log;
    auto logger = logger_t::create("default");
    logger->add_channel<channels::basic_file_mt>("info.log");
    logger->add_channel<channels::stdout_mt>();
    logger->warn("this is a test");
}
