#include "haxor/common/haxor.hh"
#include "haxor/hvm/os.hh"
#include "haxor/hvm/regs.hh"
#include "haxor/hvm/vm.hh"

#include <iostream>
#include <vector>
#include <cstdarg>
#include <stdexcept>
#include <random>
#include <cinttypes>
#include <regex>

namespace haxor {
  os::os(class vm &vm) : vm(vm) {}

  void os::syscall() {
    sp = vm.get_cpu().get_regs().read(reg_stack);
    word_t id = vm.get_cpu().get_regs().read(reg_syscall);
    word_t ret;

    switch(id) {
    case 0x01:
      ret = sc_print();
      break;

    case 0x02:
      ret = sc_printi();
      break;

    case 0x03:
      ret = sc_scan();
      break;

    case 0x04:
      ret = sc_scani();
      break;

    case 0x05:
      ret = sc_rand();
      break;

    default:
      ret = -1;
    }

    vm.get_cpu().get_regs().write(reg_syscall, ret);
  }

  word_t os::sc_print() {
    word_t addr = pop();
    std::string string = vm.get_mem().read_string(addr);
    std::cout << string;

    return 0;
  }

  word_t os::sc_printi() {
    word_t num = pop();
    std::cout << num;

    return 0;
  }

  word_t os::sc_scan() {
    word_t addr = pop();
    word_t size = pop();

    char *buffer = new char[size];
    std::cin.getline(buffer, size);
    vm.get_mem().write_string(addr, buffer);
    delete buffer;

    return 0;
  }

  word_t os::sc_scani() {
    word_t addr = pop();
    word_t value;
    word_t ret;

    ret = scanf("%" SCNd64, &value);
    vm.get_mem().write_word(addr, value);

    if (ret <= 0) {
      discard_input();
    }

    return ret;
  }

  word_t os::sc_rand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(pop(), pop());

    return dist(gen);
  }

  void os::discard_input() {
    std::cin.sync();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  // it gets next value from stack, but doesn't move real $sp register.
  // finally it works like we had prologue/epilogue with frame stack.
  word_t os::pop() {
    word_t result = vm.get_mem().read_word(sp);
    sp += sizeof(word_t);

    return result;
  }
}
