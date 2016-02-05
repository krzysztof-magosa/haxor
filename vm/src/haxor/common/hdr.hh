#ifndef HAXOR_HDR_HH
#define HAXOR_HDR_HH

#include "haxor/common/haxor.hh"

namespace haxor {
  struct hdr_t {
    int64_t magic = 0;
    int64_t version = 0;
    int64_t entry_point = 0;
    int64_t bss_size = 0;
    int64_t stack_size = 0;
    int64_t _reserved1 = 0;
    int64_t _reserved2 = 0;
    int64_t _reserved3 = 0;
  };
}

#endif
