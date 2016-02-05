#ifndef HAXOR_OPCODE_HH
#define HAXOR_OPCODE_HH

#include "haxor/common/haxor.hh"

namespace haxor {
  struct opcode_t {
    uint8_t cmd = 0;
    uint8_t flags = 0;
    uint8_t reg1 = 0;
    uint8_t reg2 = 0;
    uint8_t reg3 = 0;
    int64_t imm = 0;

    void set_reg(int reg_no, uint8_t reg);
  };

  int64_t encode_opcode(const opcode_t &opcode);
  void decode_opcode(const int64_t input, opcode_t &opcode);
}

#endif
