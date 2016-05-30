#include <iostream>
#include "gtest/gtest.h"
#include "haxor/hvm/mem.hh"

TEST(mem_tests, initial_size)
{
  haxor::mem mem;
  EXPECT_EQ(mem.get_size(), 0);
}

TEST(mem_tests, alloc)
{
  haxor::mem mem;
  mem.alloc(1024);
  EXPECT_EQ(mem.get_size(), 1024);
}

TEST(mem_tests, alloc_alignment)
{
  haxor::mem mem;
  EXPECT_THROW(mem.alloc(7), haxor::mem_misalign_error);
}

TEST(mem_tests, read_word_range_1)
{
  haxor::mem mem;
  EXPECT_THROW(mem.read_word(0), haxor::mem_range_error);
}

TEST(mem_tests, read_word_range_2)
{
  haxor::mem mem;
  mem.alloc(4096);
  EXPECT_THROW(mem.read_word(4096), haxor::mem_range_error);
}
