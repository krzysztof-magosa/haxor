#ifndef HAXOR_VM_HH
#define HAXOR_VM_HH

#include <string>

#include "haxor/common/haxor.hh"
#include "haxor/hvm/mem.hh"
#include "haxor/hvm/cpu.hh"
#include "haxor/hvm/os.hh"
#include "haxor/common/hdr.hh"
#include "haxor/common/opcode.hh"

namespace haxor {
  class vm {
    public:
    vm();
    void load_exec(const std::string &filename);
    int8_t run();
    class cpu& get_cpu();
    class mem& get_mem();
    class os& get_os();
    void exit(int8_t exit_code);

    private:
    bool running = true;
    int8_t exit_code;
    class mem mem;
    class cpu cpu;
    class os os;
    hdr_t hdr;
    opcode_t opcode;
  };
}
#endif
