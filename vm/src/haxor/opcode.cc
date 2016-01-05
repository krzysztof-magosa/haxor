#include "haxor/haxor.hh"
#include "haxor/opcode.hh"

#include <iostream>

namespace haxor {
  void decode_opcode(const int64_t input, opcode_t &opcode) {
    opcode.cmd   = input & 0x7f;
    opcode.flags = (input >> 7) & 0x03;
    opcode.reg1  = (input >> 9) & 0x3f;
    opcode.reg2  = (input >> 15) & 0x3f;
    opcode.reg3  = (input >> 21) & 0x3f;
    opcode.imm   = (input >> 27);
  }
}
