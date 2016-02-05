#include "haxor/common/haxor.hh"
#include "haxor/common/opcode.hh"

#include <stdexcept>
#include <iostream>

namespace haxor {
  void opcode_t::set_reg(const int reg_no, const uint8_t reg) {
    switch (reg_no) {
    case 1: reg1 = reg; break;
    case 2: reg2 = reg; break;
    case 3: reg3 = reg; break;
    default: throw std::runtime_error("Opcode has only regs 1-3.");
    }
  }

  int64_t encode_opcode(const opcode_t &opcode) {
    int64_t x;

    // @TODO: add & to limit size of some parameters
    x = opcode.cmd;
    x |= opcode.flags << 7;
    x |= opcode.reg1 << 9;
    x |= opcode.reg2 << 15;
    x |= opcode.reg3 << 21;
    x |= opcode.imm << 27;

    return x;
  }

  void decode_opcode(const int64_t input, opcode_t &opcode) {
    opcode.cmd   = input & 0x7f;
    opcode.flags = (input >> 7) & 0x03;
    opcode.reg1  = (input >> 9) & 0x3f;
    opcode.reg2  = (input >> 15) & 0x3f;
    opcode.reg3  = (input >> 21) & 0x3f;
    opcode.imm   = (input >> 27);
  }
}
