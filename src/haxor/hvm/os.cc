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
#include <chrono>
#include <thread>

namespace haxor {
  os::os(class vm &vm) : vm(vm) {}

  void os::syscall() {
    word_t id = vm.get_cpu().get_regs().read(reg_syscall);
    word_t ret;

    switch(id) {
    case 0x01:
      ret = sc_print_string();
      break;

    case 0x02:
      ret = sc_print_int();
      break;

    case 0x03:
      ret = sc_read_string();
      break;

    case 0x04:
      ret = sc_read_int();
      break;

    case 0x05:
      ret = sc_rand();
      break;

    case 0x06:
      ret = sc_sleep();
      break;

    case 0x07:
      ret = sc_exit();
      break;

    case 0x08:
      ret = sc_create_timer();
      break;

    default:
      ret = -1;
    }

    vm.get_cpu().get_regs().write(reg_ret0, ret);
  }

  word_t os::sc_print_string() {
    word_t addr = vm.get_cpu().get_regs().read(reg_arg0);
    std::string string = vm.get_mem().read_string(addr);
    std::cout << string;

    return 0;
  }

  word_t os::sc_print_int() {
    word_t num = vm.get_cpu().get_regs().read(reg_arg0);
    std::cout << num << std::flush;

    return 0;
  }

  word_t os::sc_read_string() {
    word_t addr = vm.get_cpu().get_regs().read(reg_arg0);
    word_t size = vm.get_cpu().get_regs().read(reg_arg1);

    char *buffer = new char[size];
    std::cin.getline(buffer, size);
    vm.get_mem().write_string(addr, buffer);
    delete[] buffer;

    return 0;
  }

  word_t os::sc_read_int() {
    word_t addr = vm.get_cpu().get_regs().read(reg_arg0);
    word_t value;
    word_t ret;

    ret = scanf("%" SCNd64, &value);
    vm.get_mem().write_word(addr, value);

    if (ret <= 0) {
      discard_input();
    }

    vm.get_cpu().get_regs().write(reg_ret1, ret);

    return value;
  }

  word_t os::sc_rand() {
    std::random_device rd;
    std::mt19937 gen(rd());
    word_t min = vm.get_cpu().get_regs().read(reg_arg0);
    word_t max = vm.get_cpu().get_regs().read(reg_arg1);
    std::uniform_int_distribution<word_t> dist(min, max);

    return dist(gen);
  }

  word_t os::sc_sleep() {
    const word_t time = vm.get_cpu().get_regs().read(reg_arg0);
    std::this_thread::sleep_for(std::chrono::milliseconds(time));

    return 0;
  }

  word_t os::sc_exit() {
    vm.exit(vm.get_cpu().get_regs().read(reg_arg0));

    return 0;
  }

  word_t os::sc_create_timer() {
    const word_t step   = vm.get_cpu().get_regs().read(reg_arg0);
    const word_t int_no = vm.get_cpu().get_regs().read(reg_arg1);

    vm.get_cpu().get_timers().emplace_back(step, int_no);

    return 0;
  }

  void os::discard_input() {
    std::cin.sync();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}
