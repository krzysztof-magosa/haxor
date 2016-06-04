#ifndef HAXOR_MEM_HH
#define HAXOR_MEM_HH

#include "haxor/common/haxor.hh"
#include <cstdint>

namespace haxor {
  class mem {
    public:
    mem();
    ~mem();
    word_t read_word(const uint64_t addr) const;
    void write_word(const uint64_t addr, const word_t value);
    std::string read_string(const uint64_t addr) const;
    void write_string(const uint64_t addr, const std::string &value);
    void alloc(const uint64_t space);
    uint64_t get_size();

    private:
    void validate_addr(const uint64_t addr) const;

    int8_t *data;
    uint64_t size;
  };
}

#endif
