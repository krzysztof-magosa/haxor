#include "haxor/haxor.hh"
#include "haxor/vm.hh"
#include "haxor/cpu.hh"

#include <iostream>

namespace haxor {
  cpu::cpu(class vm &vm) : vm(vm) {}

  void cpu::cycle() {
    decode_opcode(vm.get_mem().read_word(ip), opcode);
    ip += sizeof(word_t);
    execute(opcode);
  }

  void cpu::execute(const opcode_t &op) {
    uint64_t tmp;
    switch(op.cmd) {
    case cmd_nop:
      break;

    case cmd_exiti:
      vm.exit(op.imm);
      break;

    case cmd_syscall:
      vm.get_os().syscall();
      break;

    case cmd_add:
      regs.write(op.reg1, regs.read(op.reg2) + regs.read(op.reg3));
      break;

    case cmd_addi:
      regs.write(op.reg1, regs.read(op.reg2) + op.imm);
      break;

    case cmd_sub:
      regs.write(op.reg1, regs.read(op.reg2) - regs.read(op.reg3));
      break;

    case cmd_mult:
      regs.write(op.reg1, regs.read(op.reg2) * regs.read(op.reg3));
      break;

    case cmd_div:
      regs.write(op.reg1, regs.read(op.reg2) / regs.read(op.reg3));
      break;

    case cmd_mod:
      regs.write(op.reg1, regs.read(op.reg2) % regs.read(op.reg3));
      break;

    case cmd_lw:
      tmp = regs.read(op.reg2) + op.imm;
      regs.write(op.reg1, vm.get_mem().read_word(tmp));
      break;

    case cmd_sw:
      tmp = regs.read(op.reg1) + op.imm;
      vm.get_mem().write_word(tmp, regs.read(op.reg2));
      break;

    case cmd_lui:
      regs.write(op.reg1, op.imm << 32);
      break;

    case cmd_and:
      regs.write(op.reg1, regs.read(op.reg2) & regs.read(op.reg3));
      break;

    case cmd_andi:
      regs.write(op.reg1, regs.read(op.reg2) & op.imm);
      break;

    case cmd_or:
      regs.write(op.reg1, regs.read(op.reg2) | regs.read(op.reg3));
      break;

    case cmd_ori:
      regs.write(op.reg1, regs.read(op.reg2) | op.imm);
      break;

    case cmd_xor:
      regs.write(op.reg1, regs.read(op.reg2) ^ regs.read(op.reg3));
      break;

    case cmd_nor:
      regs.write(op.reg1, ~(regs.read(op.reg2) | regs.read(op.reg3)));
      break;

    case cmd_slt:
      regs.write(op.reg1, regs.read(op.reg2) < regs.read(op.reg3) ? 1 : 0);
      break;

    case cmd_slti:
      regs.write(op.reg1, regs.read(op.reg2) < op.imm ? 1 : 0);
      break;

    case cmd_slli:
      regs.write(op.reg1, regs.read(op.reg2) << op.imm);
      break;

    case cmd_srli:
      regs.write(op.reg1, regs.read(op.reg2) >> op.imm);
      break;

    case cmd_sll:
      regs.write(op.reg1, regs.read(op.reg2) << regs.read(op.reg3));
      break;

    case cmd_srl:
      regs.write(op.reg1, regs.read(op.reg2) >> regs.read(op.reg3));
      break;

    case cmd_beq:
      if (regs.read(op.reg1) == regs.read(op.reg2)) {
        branch(op.imm);
      }
      break;

    case cmd_beql:
      if (regs.read(op.reg1) == regs.read(op.reg2)) {
        link();
        branch(op.imm);
      }
      break;

    case cmd_bne:
      if (regs.read(op.reg1) != regs.read(op.reg2)) {
        branch(op.imm);
      }
      break;

    case cmd_bnel:
      if (regs.read(op.reg1) != regs.read(op.reg2)) {
        link();
        branch(op.imm);
      }
      break;

    case cmd_j:
      jump(op.imm);
      break;

    case cmd_jr:
      ip = regs.read(op.reg1);
      break;

    case cmd_jal:
      link();
      jump(regs.read(op.reg1));
      break;

    default:
      std::cerr << "BROKEN OPCODE" << std::endl;
    }
  }

  void cpu::set_ip(const uint64_t ip) {
    this->ip = ip;
  }

  regs& cpu::get_regs() {
    return regs;
  }

  void cpu::link() {
    regs.write(reg_return, ip);
  }

  void cpu::branch(const uint64_t target) {
    ip += target * sizeof(word_t);
  }

  void cpu::jump(const uint64_t target) {
    ip = target * sizeof(word_t);
  }
}
