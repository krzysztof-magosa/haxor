#ifndef HAXOR_HDR_HH
#define HAXOR_HDR_HH

#include "haxor/common/haxor.hh"

namespace haxor {
  struct hdr_t {
    uint64_t magic = 0;
    uint64_t version = 0;
    uint64_t entry_point = 0;

    uint64_t text_begin = 0;
    uint64_t text_size = 0;

    uint64_t data_begin = 0;
    uint64_t data_size = 0;

    uint64_t bss_begin = 0;
    uint64_t bss_size = 0;

    uint64_t _reserved1 = 0;
  };
}

#endif
