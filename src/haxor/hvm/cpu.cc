#include "haxor/common/haxor.hh"
#include "haxor/common/cmd.hh"
#include "haxor/hvm/vm.hh"
#include "haxor/hvm/cpu.hh"

#include <iostream>

namespace haxor {
  cpu::cpu(class vm &vm) : vm(vm) {}

  void cpu::cycle() {
    if (!vm.get_mem().get_page(ip).get_attrs().exec) {
      throw segfault_error();
    }

    decode_opcode(vm.get_mem().read_word(ip), opcode);
    ip += sizeof(word_t);
    execute(opcode);
  }

  void cpu::execute(const opcode_t &op) {
    uint64_t tmp;

    switch(op.cmd) {
    case cmd_nop:
      break;

    case cmd_syscall:
      vm.get_os().syscall();
      break;

    case cmd_add:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) + regs.read(op.reg3));
      break;

    case cmd_addi:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) + op.imm);
      break;

    case cmd_sub:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) - regs.read(op.reg3));
      break;

    case cmd_mult:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) * regs.read(op.reg3));
      break;

    case cmd_div:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) / regs.read(op.reg3));
      break;

    case cmd_mod:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) % regs.read(op.reg3));
      break;

    case cmd_lw:
      validate_reg_write(op.reg1);
      tmp = regs.read(op.reg2) + op.imm;
      if (!vm.get_mem().get_page(tmp).get_attrs().read) {
        throw segfault_error();
      }
      regs.write(op.reg1, vm.get_mem().read_word(tmp));
      break;

    case cmd_sw:
      tmp = regs.read(op.reg1) + op.imm;
      if (!vm.get_mem().get_page(tmp).get_attrs().write) {
        throw segfault_error();
      }
      vm.get_mem().write_word(tmp, regs.read(op.reg2));

      break;

    case cmd_lui:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, op.imm << 32);
      break;

    case cmd_and:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) & regs.read(op.reg3));
      break;

    case cmd_andi:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) & op.imm);
      break;

    case cmd_or:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) | regs.read(op.reg3));
      break;

    case cmd_ori:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) | op.imm);
      break;

    case cmd_xor:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) ^ regs.read(op.reg3));
      break;

    case cmd_nor:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, ~(regs.read(op.reg2) | regs.read(op.reg3)));
      break;

    case cmd_slt:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) < regs.read(op.reg3) ? 1 : 0);
      break;

    case cmd_slti:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) < op.imm ? 1 : 0);
      break;

    case cmd_slli:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) << op.imm);
      break;

    case cmd_srli:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) >> op.imm);
      break;

    case cmd_sll:
      validate_reg_write(op.reg1);
      regs.write(op.reg1, regs.read(op.reg2) << regs.read(op.reg3));
      break;

    case cmd_srl:
      validate_reg_write(op.reg1);
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
      jump(op.imm);
      break;

    case cmd_jalr:
      link();
      ip = regs.read(op.reg1);
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

  void cpu::validate_reg_write(const uint8_t reg) {
    //
  }
}
