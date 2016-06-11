#ifndef HAXOR_REGS_HH
#define HAXOR_REGS_HH

#include <vector>
#include "haxor/common/haxor.hh"

namespace haxor {
  enum special_reg {
    reg_zero    = 0,
    reg_accumulator = 1,
    reg_stack   = 61,
    reg_return  = 62,
    reg_syscall = 63
  };

  class regs {
    public:
    regs();
    word_t read(const size_t id) const;
    void write(const size_t id, const word_t value);
    word_t add(const size_t id, const word_t value);

    private:
    void validate_id(const size_t id) const;

    std::vector<word_t> data;
  };
}

#endif
