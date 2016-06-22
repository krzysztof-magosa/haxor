#include "haxor/common/haxor.hh"
#include "haxor/hvm/mem.hh"
#include <cstdlib>
#include <cstring>
#include <string>

namespace haxor {
  mem_page::mem_page(const uint64_t size) : data(new int8_t[size]), size(size) {
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

  mem_addr::mem_addr(const uint64_t page, const uint64_t offset) : page(page), offset(offset) {
  }

  mem::mem() {
    data = NULL;
    size = 0;
  }

  mem::~mem() {
    free(data);
  }

  word_t mem::read_word(const uint64_t addr) const {
    validate_addr(addr);

    mem_addr maddr = convert_addr(addr);
    return pages.at(maddr.page).read_word(maddr.offset);
  }

  void mem::write_word(const uint64_t addr, const word_t value) {
    validate_addr(addr);

    mem_addr maddr = convert_addr(addr);
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

  void mem::alloc(const uint64_t space) {
    if (space % sizeof(word_t) != 0) {
      throw mem_misalign_error();
    }

    const uint64_t aligned_size = size + (space + (page_size - (space % page_size)));
    const uint64_t number_of_pages = aligned_size / page_size;

    if (number_of_pages < pages.size()) {
      throw std::invalid_argument("You cannot deallocate memory.");
    }

    const uint64_t prev_pages = pages.size();

    for (uint64_t i = prev_pages; i < number_of_pages; i++) {
      mem_page page(page_size);
      pages.push_back(page);
    }

    size += space;
  }

  uint64_t mem::get_size() {
    return size;
  }

  void mem::validate_addr(const uint64_t addr) const {
    // avoid overflow of unsigned int.
    if (size < sizeof(word_t) || addr > size - sizeof(word_t)) {
      throw mem_range_error();
    }
  }

  mem_addr mem::convert_addr(const uint64_t addr) const {
    const uint64_t lowest_address = addr - (addr % page_size);
    const uint64_t page = lowest_address / page_size;
    const uint64_t offset = addr - lowest_address;

    return mem_addr(page, offset);
  }
}
