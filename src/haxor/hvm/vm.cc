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

    exe.open(filename, std::ios::binary | std::ifstream::ate);

    if (exe.fail()) {
      throw haxe_open_error();
    }

    exe.seekg(0, std::ios::beg);

    exe.read(reinterpret_cast<char*>(&hdr), sizeof(hdr_t));
    if (hdr.magic != magic) {
      throw hdr_magic_error();
    }

    if (hdr.version != version) {
      throw hdr_version_error();
    }

    const uint64_t stack_size = 512 * page_size;

    mem.alloc_range(0, hdr.text_begin, mem_page_attrs(true, true, false));

    if (hdr.text_size) {
      mem.alloc_range(hdr.text_begin, hdr.text_size, mem_page_attrs(false, false, true));
    }

    if (hdr.data_size) {
      mem.alloc_range(hdr.data_begin, hdr.data_size, mem_page_attrs(true, true, false));
    }

    if (hdr.bss_size) {
      mem.alloc_range(hdr.bss_begin,  hdr.bss_size, mem_page_attrs(true, true, false));
    }

    mem.alloc_range(mem.max_size() + 1 - stack_size, stack_size, mem_page_attrs(true, true, false));

    // guard page below stack.
    mem.alloc_range(mem.max_size() + 1 - stack_size - page_size, page_size, mem_page_attrs(false, false, false));

    for (uint64_t i = hdr.text_begin; i < hdr.text_begin + hdr.text_size; i += sizeof(word_t)) {
      word_t x;
      exe.read(reinterpret_cast<char*>(&x), sizeof(word_t));
      mem.write_word(i, x);
    }

    for (uint64_t i = hdr.data_begin; i < hdr.data_begin + hdr.data_size; i += sizeof(word_t)) {
      word_t x;
      exe.read(reinterpret_cast<char*>(&x), sizeof(word_t));
      mem.write_word(i, x);
    }
  }

  int8_t vm::run() {
    cpu.set_ip(hdr.entry_point);
    cpu.get_regs().write(reg_stack, mem.max_size() - 1);

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
