#ifndef HAXOR_REGS_HH
#define HAXOR_REGS_HH

#include <vector>
#include "haxor/common/haxor.hh"

namespace haxor {
  enum special_reg {
    reg_zero    = 0,
    reg_accumulator = 1,
    reg_arg0 = 2,
    reg_arg1 = 3,
    reg_arg2 = 4,
    reg_arg3 = 5,
    reg_arg4 = 6,
    reg_arg5 = 7,
    reg_arg6 = 8,
    reg_arg7 = 9,
    reg_arg8 = 10,
    reg_arg9 = 11,
    reg_ret0 = 22,
    reg_ret1 = 23,
    reg_frame   = 60,
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
