#ifndef HAXOR_CPU_HH
#define HAXOR_CPU_HH

#include "haxor/common/haxor.hh"
#include "haxor/hvm/regs.hh"
#include "haxor/common/opcode.hh"

#include <chrono>
#include <thread>

namespace haxor {
  class vm;

  class timer {
    public:
    timer(const word_t step, const word_t int_no);
    void schedule();
    void tick();
    bool check();
    word_t get_int_no();

    private:
    const word_t step;
    const word_t int_no;

    word_t remaining_ticks;
  };

  enum class memory_segment {
    ivt,
    code,
    data,
    stack
  };

  class cpu {
    public:
    explicit cpu(class vm &vm);
    void cycle();
    void execute(const opcode_t &op);
    void set_ip(const uint64_t ip);
    void start_sys_timer();
    void stop_sys_timer();
    class regs& get_regs();
    std::vector<timer> &get_timers();

    private:
    void link();
    void branch(const uint64_t target);
    void jump(const uint64_t target);
    void validate_reg_write(const uint8_t reg);
    void perform_int(const word_t int_no);

    void sys_timer_loop();

    class vm &vm;
    class regs regs;
    opcode_t opcode;
    uint64_t ip;

    std::vector<timer> timers;
    bool sys_timer_kill = false;
    std::thread sys_timer;
  };
}

#endif
