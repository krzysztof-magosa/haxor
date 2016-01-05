#ifndef HAXOR_CPU_HH
#define HAXOR_CPU_HH

#include "haxor/haxor.hh"
#include "haxor/regs.hh"
#include "haxor/opcode.hh"

namespace haxor {
  class vm;

  enum cmd {
    cmd_nop     = 0x00,
    cmd_exiti   = 0x01,
    cmd_syscall = 0x02,

    cmd_add     = 0x10,
    cmd_addi    = 0x11,
    cmd_sub     = 0x12,
    cmd_mult    = 0x13,
    cmd_div     = 0x14,
    cmd_mod     = 0x15,

    cmd_lw      = 0x20,
    cmd_sw      = 0x21,
    cmd_lui     = 0x22,

    cmd_and     = 0x30,
    cmd_andi    = 0x31,
    cmd_or      = 0x32,
    cmd_ori     = 0x33,
    cmd_xor     = 0x34,
    cmd_nor     = 0x35,
    cmd_slt     = 0x36,
    cmd_slti    = 0x37,

    cmd_slli    = 0x40,
    cmd_srli    = 0x41,
    cmd_sll     = 0x42,
    cmd_srl     = 0x43,

    cmd_beq     = 0x50,
    cmd_beql    = 0x51,
    cmd_bne     = 0x52,
    cmd_bnel    = 0x53,
    cmd_j       = 0x54,
    cmd_jr      = 0x55,
    cmd_jal     = 0x56
  };

  class cpu {
    public:
    cpu(vm &vm);
    void cycle();
    void execute(const opcode_t &op);
    void set_ip(const uint64_t ip);
    regs& get_regs();

    private:
    void link();
    void branch(const uint64_t target);
    void jump(const uint64_t target);

    vm &vm;
    regs regs;
    opcode_t opcode;
    uint64_t ip;
  };
}

#endif
