#ifndef HAXOR_CPU_HH
#define HAXOR_CPU_HH

#include "haxor/common/haxor.hh"
#include "haxor/hvm/regs.hh"
#include "haxor/common/opcode.hh"

namespace haxor {
  class vm;

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
