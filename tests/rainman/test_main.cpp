#include <gtest/gtest.h>
#include <rainman/core/RainmanLog.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/sink.h>

int main(int argc, char **argv)
{
	RainmanLog::init("test_logs");
	spdlog::get("rainman")->sinks()[0]->set_level(spdlog::level::warn);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
