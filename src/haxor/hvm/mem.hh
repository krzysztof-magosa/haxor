#ifndef HAXOR_MEM_HH
#define HAXOR_MEM_HH

#include "haxor/common/haxor.hh"
#include <cstdint>
#include <map>

namespace haxor {
  class mem_page_attrs {
    public:
    mem_page_attrs(const bool read, const bool write, const bool exec);
    const bool read;
    const bool write;
    const bool exec;
  };

  class mem_page {
    public:
    mem_page(const uint64_t size, const mem_page_attrs &attrs);
    word_t read_word(const uint64_t addr) const;
    void write_word(const uint64_t addr, const word_t value);
    void validate_addr(const uint64_t addr) const;
    uint64_t get_size() const;
    const mem_page_attrs &get_attrs() const;

    private:
    int8_t * const data;
    const mem_page_attrs attrs;
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
    void alloc_range(const uint64_t begin, const uint64_t size, const mem_page_attrs &attrs);
    mem_addr convert_addr(const uint64_t addr) const;
    uint64_t max_size();
    const mem_page &get_page(const uint64_t addr);

    private:
    void validate_mem_addr(const mem_addr &maddr) const;

    std::map<uint64_t, mem_page> pages;
  };
}

#endif
