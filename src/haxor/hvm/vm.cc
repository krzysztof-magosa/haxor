#include <fstream>
#include <iostream>

#include "haxor/common/haxor.hh"
#include "haxor/common/hdr.hh"
#include "haxor/hvm/vm.hh"
#include "haxor/common/opcode.hh"

namespace haxor {
  vm::vm() : cpu(*this), os(*this) {
  }

  void vm::load_exec(const std::string &filename) {
    std::ifstream exe;
    std::uint64_t esize;

    exe.open(filename, std::ios::binary | std::ifstream::ate);

    if (exe.fail()) {
      throw haxe_open_error();
    }

    esize = static_cast<uint64_t>(exe.tellg()) - sizeof(hdr_t);
    exe.seekg(0, std::ios::beg);

    exe.read(reinterpret_cast<char*>(&hdr), sizeof(hdr_t));
    if (hdr.magic != magic) {
      throw hdr_magic_error();
    }

    if (hdr.version != version) {
      throw hdr_version_error();
    }

    mem.alloc(ivt_size + esize + hdr.bss_size + hdr.stack_size);

    uint64_t index = ivt_size;
    while (true) {
      word_t x;
      exe.read(reinterpret_cast<char*>(&x), sizeof(word_t));

      if (exe.eof()) {
        break;
      }

      mem.write_word(index, x);
      index += sizeof(word_t);
    }
  }

  int8_t vm::run() {
    cpu.set_ip(hdr.entry_point);
    cpu.get_regs().write(reg_stack, mem.get_size());

    const uint64_t code_addr = ivt_size;
    const uint64_t data_addr = code_addr + hdr.text_size;
    const uint64_t stack_addr = data_addr + hdr.data_size;
    cpu.get_regs().write(reg_code_segment, code_addr);
    cpu.get_regs().write(reg_data_segment, data_addr);
    cpu.get_regs().write(reg_stack_segment, stack_addr);

    while (running) {
      cpu.cycle();
    }

    return exit_code;
  }

  os& vm::get_os() {
    return os;
  }

  cpu& vm::get_cpu() {
    return cpu;
  }

  mem& vm::get_mem() {
    return mem;
  }

  void vm::exit(int8_t exit_code) {
    this->exit_code = exit_code;
    running = false;
  }
}
