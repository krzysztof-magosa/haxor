#ifndef HAXOR_OS_HH
#define HAXOR_OS_HH

#include "haxor/hvm/haxor.hh"
#include <vector>
#include <string>

namespace haxor {
  class vm;

  class os {
    public:
    os(vm &vm);
    void syscall();

    private:
    word_t sc_printf();
    word_t sc_scanf();
    word_t sc_rand();

    word_t pop();
    void discard_input();

    word_t sp;
    vm &vm;
  };
}

#endif
