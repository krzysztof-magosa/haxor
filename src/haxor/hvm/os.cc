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

    case 0x09:
      ret = sc_enable_timer();
      break;

    case 0x0a:
      ret = sc_disable_timer();
      break;

    case 0x0c:
      ret = sc_steady_time();
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

    return vm.get_cpu().get_timers().size() - 1;
  }

  word_t os::sc_enable_timer() {
    const word_t id = vm.get_cpu().get_regs().read(reg_arg0);
    vm.get_cpu().get_timers().at(id).enable();
    return 0;
  }

  word_t os::sc_disable_timer() {
    const word_t id = vm.get_cpu().get_regs().read(reg_arg0);
    vm.get_cpu().get_timers().at(id).disable();
    return 0;
  }

  word_t os::sc_steady_time() {
    const word_t return_as = vm.get_cpu().get_regs().read(reg_arg0);
    auto now = std::chrono::steady_clock::now();

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::hours> hours;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::minutes> minutes;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::seconds> seconds;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> milliseconds;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds> microseconds;
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds> nanoseconds;

    switch (return_as) {
    case 0:
      hours = std::chrono::time_point_cast<std::chrono::hours>(now);
      return hours.time_since_epoch().count();
      break;
    case 1:
      minutes = std::chrono::time_point_cast<std::chrono::minutes>(now);
      return minutes.time_since_epoch().count();
      break;
    case 2:
      seconds = std::chrono::time_point_cast<std::chrono::seconds>(now);
      return seconds.time_since_epoch().count();
      break;
    case 3:
      milliseconds = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
      return milliseconds.time_since_epoch().count();
      break;
    case 4:
      microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(now);
      return microseconds.time_since_epoch().count();
      break;
    case 5:
      nanoseconds = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
      return nanoseconds.time_since_epoch().count();
      break;
    default:
      throw std::invalid_argument("Invalid return_as parameter.");
    }
  }

  void os::discard_input() {
    std::cin.sync();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
}
