
#include <pride/pride.hpp>

int main()
{
    using namespace pride::log;
    auto logger = logger_t::create("default");
    logger
        ->add_channel<channels::basic_file_mt>("info.log", true)
         .add_channel<channels::stdout_mt>()
         .set_pattern("%n: %t -> |%l| %i%v");

    logger->trace("this is a trace message");
    logger->info("this is an info message");
    indent_t::increase();
    logger->debug("this is a debug message");
    logger->warn("this is a warning message");
    indent_t::decrease();
    logger->error("this is an error message");
    logger->crit("this is a critical error message");

    logger->info("this {} a {} of {} library {} function", "is", "test", "fmt", "insert");
}
