#include "gtest/gtest.h"
#include "pgext/list.h"

TEST(MyTest, Whatever) {
  pgext::list<int> my_list;
  my_list.push_back(1);

  EXPECT_EQ(my_list.size(), 1);
}
