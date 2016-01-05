#ifndef HAXOR_HH
#define HAXOR_HH

#include <cstdint>
#include <stdexcept>

namespace haxor {
  typedef int64_t word_t;
  constexpr size_t word_size_t = sizeof(word_t);

  const int64_t magic = 0x7265887982;

  const uint64_t ivt_num = 128;
  constexpr uint64_t ivt_size = word_size_t * ivt_num;

  class hdr_magic_error : public std::exception {};
  class regs_range_error : public std::exception {};
  class mem_misalign_error : public std::exception {};
  class mem_range_error : public std::exception {};
}

#endif
