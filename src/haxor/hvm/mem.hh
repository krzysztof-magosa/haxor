#ifndef HAXOR_MEM_HH
#define HAXOR_MEM_HH

#include "haxor/common/haxor.hh"
#include <cstdint>
#include <vector>

namespace haxor {
  class mem_page {
    public:
    explicit mem_page(const uint64_t size);
    word_t read_word(const uint64_t addr) const;
    void write_word(const uint64_t addr, const word_t value);
    void validate_addr(const uint64_t addr) const;
    uint64_t get_size() const;

    private:
    int8_t * const data;
    const uint64_t size;
  };

  class mem_addr {
    public:
    mem_addr(const uint64_t page, const uint64_t offset);

    const uint64_t page;
    const uint64_t offset;
  };

  class mem {
    public:
    mem();
    ~mem();
    word_t read_word(const uint64_t addr) const;
    void write_word(const uint64_t addr, const word_t value);
    std::string read_string(const uint64_t addr) const;
    void write_string(const uint64_t addr, const std::string &value);
    void alloc(const uint64_t space);
    mem_addr convert_addr(const uint64_t addr) const;
    uint64_t get_size();

    private:
    void validate_addr(const uint64_t addr) const;

    std::vector<mem_page> pages;

    int8_t *data;
    uint64_t size;
  };
}

#endif
