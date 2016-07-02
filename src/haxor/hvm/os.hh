#ifndef HAXOR_OS_HH
#define HAXOR_OS_HH

#include "haxor/common/haxor.hh"
#include <vector>
#include <string>

namespace haxor {
  class vm;

  class os {
    public:
    explicit os(class vm &vm);
    void syscall();

    private:
    word_t sc_print_string();
    word_t sc_print_int();
    word_t sc_read_string();
    word_t sc_read_int();
    word_t sc_rand();
    word_t sc_sleep();
    word_t sc_exit();
    word_t sc_create_timer();
    word_t sc_enable_timer();
    word_t sc_disable_timer();

    word_t pop();
    void discard_input();

    class vm &vm;
  };
}

#endif
