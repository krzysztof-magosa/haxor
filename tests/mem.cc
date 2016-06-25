#include <iostream>
#include "gtest/gtest.h"
#include "haxor/hvm/mem.hh"

TEST(mem_tests, read_word_range_1)
{
  haxor::mem mem;
  EXPECT_THROW(mem.read_word(0), haxor::mem_range_error);
}

TEST(mem_tests, read_word_range_2)
{
  haxor::mem mem;
  mem.alloc_range(0, 4096, haxor::mem_page_attrs(true, true, true));
  EXPECT_THROW(mem.read_word(4096), haxor::mem_range_error);
}

TEST(mem_tests, write_string)
{
  haxor::mem mem;
  haxor::word_t addr = 1024;

  mem.alloc_range(0, 4096, haxor::mem_page_attrs(true, true, true));
  mem.write_string(addr, "Haxor");

  // strings in Haxor are also aligned to 8 bytes.
  EXPECT_EQ('H', mem.read_word(addr));
  EXPECT_EQ('a', mem.read_word(addr+=8));
  EXPECT_EQ('x', mem.read_word(addr+=8));
  EXPECT_EQ('o', mem.read_word(addr+=8));
  EXPECT_EQ('r', mem.read_word(addr+=8));
}

TEST(mem_tests, read_string)
{
  haxor::mem mem;
  mem.alloc_range(0, 4096, haxor::mem_page_attrs(true, true, true));
  mem.write_string(2048, "Virtual");
  const std::string result = mem.read_string(2048);

  EXPECT_EQ("Virtual", result);
}
