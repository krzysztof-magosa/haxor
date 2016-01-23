#ifndef HAXOR_VM_HH
#define HAXOR_VM_HH

#include <string>

#include "haxor/hvm/haxor.hh"
#include "haxor/hvm/mem.hh"
#include "haxor/hvm/cpu.hh"
#include "haxor/hvm/os.hh"
#include "haxor/hvm/hdr.hh"
#include "haxor/hvm/opcode.hh"

namespace haxor {
  class vm {
    public:
    vm();
    void load_exec(const std::string filename);
    int8_t run();
    cpu& get_cpu();
    mem& get_mem();
    os& get_os();
    void exit(int8_t exit_code);

    private:
    bool running = true;
    int8_t exit_code;
    mem mem;
    cpu cpu;
    os os;
    hdr_t hdr;
    opcode_t opcode;
  };
}

#endif
