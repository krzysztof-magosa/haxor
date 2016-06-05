#include "gtest/gtest.h"
#include "haxor/hvm/regs.hh"

TEST(regs_test, ignore_writes_to_zero)
{
  haxor::regs regs;
  regs.write(0, 123); // write 123 to register 0
  ASSERT_EQ(regs.read(0), 0); // still zero
}

TEST(regs_test, read_write)
{
  haxor::regs regs;
  regs.write(5, 123);
  ASSERT_EQ(123, regs.read(5));
}
