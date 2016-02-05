#include "haxor/common/haxor.hh"
#include "haxor/hvm/os.hh"
#include "haxor/hvm/regs.hh"
#include "haxor/hvm/vm.hh"
#include "haxor/hvm/format.hh"

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
      ret = sc_printf();
      break;

    case 0x02:
      ret = sc_scanf();
      break;

    case 0x03:
      ret = sc_rand();
      break;

    default:
      ret = -1;
    }

    vm.get_cpu().get_regs().write(reg_syscall, ret);
  }

  word_t os::sc_printf() {
    word_t fd = pop(); // ignored at the moment
    uint64_t fmt_addr = pop();
    std::string fmt = vm.get_mem().read_string(fmt_addr);
    std::vector<fmt_token> tokens = get_fmt_tokens(fmt);

    std::string temp;
    for (fmt_token item : tokens) {
      switch (item.type) {
      case fmt_token_type::integer:
        temp = "%" + item.flags + item.width + item.precision + "j" + item.specifier;
        std::printf(temp.c_str(), static_cast<intmax_t>(pop()));
        break;

      case fmt_token_type::real:
        // not used at the moment
        pop();
        break;

      case fmt_token_type::text:
        std::printf(concat_fmt_token(item).c_str(), vm.get_mem().read_string(pop()).c_str());
        break;

      case fmt_token_type::letter:
        // not used at the moment
        pop();
        break;

      case fmt_token_type::pointer:
        // not used at the moment
        pop();
        break;

      case fmt_token_type::free_text:
        std::printf("%s", item.free_text.c_str());
        break;
      }
    }

    return 0;
  }

  word_t os::sc_scanf() {
    word_t ret = 0;

    try {
      word_t fd = pop(); // ignored at the moment
      uint64_t fmt_addr = pop();
      std::string fmt = vm.get_mem().read_string(fmt_addr);
      std::vector<fmt_token> tokens = get_fmt_tokens(fmt);

      std::string temp;
      std::string width;
      char *chars;
      intmax_t integer;

      for (fmt_token item : tokens) {
        switch (item.type) {
        case fmt_token_type::integer:
          // 'j' stands for intmax_t
          temp = "%" + item.flags + item.width + item.precision + "j" + item.specifier;
          ret += std::scanf(temp.c_str(), &integer);
          vm.get_mem().write_word(pop(), integer);
          break;

        case fmt_token_type::real:
          // todo
          break;

        case fmt_token_type::text:
          temp = "%" + item.flags + item.width + item.precision + item.length + item.specifier;
          chars = new char[item.width_n+1];
          ret += std::scanf(temp.c_str(), chars);
          vm.get_mem().write_string(pop(), std::string(chars));
          delete[] chars;
          break;

        case fmt_token_type::letter:
          // not used at the moment
          pop();
          break;

        case fmt_token_type::pointer:
          // not used at the moment
          pop();
          break;

        case fmt_token_type::free_text:
          break;
        }
      }
    }
    catch (std::invalid_argument) {
      // caused by invalid format - e.g. bad width.
      ret = -1;
    }

    if (ret <= 0) {
      discard_input();
    }

    return ret;
  }

  void os::discard_input() {
    std::cin.sync();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  word_t os::sc_rand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(pop(), pop());

    return dist(gen);
  }

  // it gets next value from stack, but doesn't move real $sp register.
  // finally it works like we had prologue/epilogue with frame stack.
  word_t os::pop() {
    word_t result = vm.get_mem().read_word(sp);
    sp += sizeof(word_t);

    return result;
  }
}
