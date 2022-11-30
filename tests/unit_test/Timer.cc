#include <gtest/gtest.h>
#include "Timer.hpp"

TEST(timer, timeout) {
  Timer t(1);
	sleep(1);
  EXPECT_EQ(true, t.TimeOver());
}

TEST(timer, nottimeout) {
  Timer t(1);
  EXPECT_EQ(false, t.TimeOver());
}
