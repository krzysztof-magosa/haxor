#ifndef HAXOR_HDR_HH
#define HAXOR_HDR_HH

#include "haxor/haxor.hh"

namespace haxor {
  struct hdr_t {
    int64_t magic;
    int64_t version;
    int64_t entry_point;
    int64_t bss_size;
    int64_t stack_size;
    int64_t _reserved1;
    int64_t _reserved2;
    int64_t _reserved3;
  };
}

#endif
