//
// Created by Rong Zhou on 2018-08-03.
//

#include "gtest/gtest.h"
#include "user.h"

using namespace ats;

TEST(User, get_username) {
    User user("rongzhou");
    EXPECT_EQ(user.get_username(), "rongzhou");
}