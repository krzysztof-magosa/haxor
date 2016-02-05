#ifndef HAXOR_COMMON_HAXOR_HH
#define HAXOR_COMMON_HAXOR_HH

#include <cstdint>
#include <stdexcept>
#include <string>

namespace haxor {
  typedef int64_t word_t;

  const int64_t magic = 0x7265887982;
  const int64_t major = 0;
  const int64_t minor = 6;
  const int64_t patch = 1;
  constexpr int64_t version = (major << 16) | (minor << 8) | patch;

  const uint64_t ivt_num = 128;
  constexpr uint64_t ivt_size = sizeof(word_t) * ivt_num;

  class haxe_open_error : public std::exception {
    virtual const char* what() const throw() {
      return "Cannot open specified file. Check path and try again.";
    }
  };

  class hdr_magic_error : public std::exception {
    virtual const char* what() const throw() {
      return "Incorrect magic number. You are probably trying to run incorrect file.";
    }
  };

  class hdr_version_error : public std::exception {
    virtual const char* what() const throw() {
      return "Incorrect version number. Program was compiled for different version of Haxor.";
    }
  };

  class regs_range_error : public std::exception {};
  class mem_misalign_error : public std::exception {};
  class mem_range_error : public std::exception {};
}

#endif
