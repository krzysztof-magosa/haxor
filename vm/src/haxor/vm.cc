#include <fstream>
#include <iostream>

#include "haxor/haxor.hh"
#include "haxor/hdr.hh"
#include "haxor/vm.hh"
#include "haxor/opcode.hh"

namespace haxor {
  vm::vm() : cpu(*this), os(*this) {
  }

  void vm::load_exec(const std::string filename) {
    std::ifstream exe;
    std::uint64_t esize;

    exe.open(filename, std::ios::binary | std::ifstream::ate);
    esize = static_cast<uint64_t>(exe.tellg()) - sizeof(hdr_t);
    exe.seekg(0, std::ios::beg);

    exe.read(reinterpret_cast<char*>(&hdr), sizeof(hdr_t));
    if (hdr.magic != magic) {
      throw hdr_magic_error();
    }

    mem.alloc(ivt_size + esize + hdr.bss_size + hdr.stack_size);

    uint64_t index = ivt_size;
    while (true) {
      word_t x;
      exe.read(reinterpret_cast<char*>(&x), word_size_t);

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