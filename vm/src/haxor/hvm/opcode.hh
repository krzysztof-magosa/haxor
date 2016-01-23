#ifndef HAXOR_OPCODE_HH
#define HAXOR_OPCODE_HH

#include "haxor/hvm/haxor.hh"

namespace haxor {
  struct opcode_t {
    uint8_t cmd;
    uint8_t flags;
    uint8_t reg1;
    uint8_t reg2;
    uint8_t reg3;
    int64_t imm;
  };

  void decode_opcode(const int64_t input, opcode_t &opcode);
}

#endif
