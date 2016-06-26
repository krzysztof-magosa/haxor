#include "haxor/common/haxor.hh"
#include "haxor/hvm/mem.hh"
#include <cstdlib>
#include <cstring>
#include <string>
#include <cassert>
#include <limits>

namespace haxor {
  mem_page_attrs::mem_page_attrs(const bool read, const bool write, const bool exec) : read(read), write(write), exec(exec) {
  }

  mem_page::mem_page(const uint64_t size, const mem_page_attrs &attrs) : data(new int8_t[size]), attrs(attrs), size(size) {
    memset(data, 0, size);
  }

  word_t mem_page::read_word(const uint64_t addr) const {
    validate_addr(addr);
    word_t temp;
    memcpy(&temp, data + addr, sizeof(word_t));
    return temp;
  }

  void mem_page::write_word(const uint64_t addr, const word_t value) {
    validate_addr(addr);
    memcpy(data + addr, &value, sizeof(word_t));
  }

  void mem_page::validate_addr(const uint64_t addr) const {
    // avoid overflow of unsigned int.
    if (addr > size - sizeof(word_t)) {
      throw mem_range_error();
    }
  }

  uint64_t mem_page::get_size() const {
    return size;
  }

  const mem_page_attrs &mem_page::get_attrs() const {
    return attrs;
  }

  mem_addr::mem_addr(const uint64_t page, const uint64_t offset) : page(page), offset(offset) {
  }

  mem::mem() {
  }

  mem::~mem() {
  }

  word_t mem::read_word(const uint64_t addr) const {
    mem_addr maddr = convert_addr(addr);
    validate_mem_addr(maddr);
    return pages.at(maddr.page).read_word(maddr.offset);
  }

  void mem::write_word(const uint64_t addr, const word_t value) {
    mem_addr maddr = convert_addr(addr);
    validate_mem_addr(maddr);
    return pages.at(maddr.page).write_word(maddr.offset, value);
  }

  std::string mem::read_string(const uint64_t addr) const {
    std::string result = "";

    for (uint64_t offset = 0; ; offset += sizeof(word_t)) {
      word_t item = read_word(addr + offset);
      if (item == 0) {
        break;
      }

      result += item;
    }

    return result;
  }

  void mem::write_string(const uint64_t addr, const std::string &value) {
    size_t offset = 0;
    for (size_t i = 0; i < value.size(); i++) {
      write_word(addr + offset, value[i]);
      offset += sizeof(word_t);
    }
    write_word(addr + offset, 0); // ending '\0'
  }

  void mem::alloc_range(const uint64_t begin, const uint64_t size, const mem_page_attrs &attrs) {
    if (begin % page_size != 0) {
      // std::cout << begin << std::endl;
      throw std::invalid_argument("Begin address must be page aligned.");
    }

    const uint64_t aligned_size = ((size + page_size - 1) / page_size) * page_size;
    const uint64_t page_from = begin / page_size;
    const uint64_t page_to   = page_from + (aligned_size / page_size) - 1;

    assert(page_to >= page_from);

    for (uint64_t i = page_from; i <= page_to; i++) {
      pages.emplace(i, mem_page(page_size, attrs));
    }
  }

  const mem_page &mem::get_page(const uint64_t addr) {
    const uint64_t aligned_addr = addr - (addr % page_size);
    return pages.at(aligned_addr / page_size);
  }

  mem_addr mem::convert_addr(const uint64_t addr) const {
    const uint64_t lowest_address = addr - (addr % page_size);
    const uint64_t page = lowest_address / page_size;
    const uint64_t offset = addr - lowest_address;

    return mem_addr(page, offset);
  }

  uint64_t mem::max_size() {
    return std::numeric_limits<int64_t>::max();
  }

  void mem::validate_mem_addr(const mem_addr &maddr) const {
    if (pages.find(maddr.page) == pages.end()) {
      throw mem_range_error();
    }

    // offset within page is checked there.
  }
}
