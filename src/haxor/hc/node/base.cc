#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    bool base::is_a(type type) {
      return (get_type() == type);
    }

    word_t base::get_size() const {
      return size;
    }

    word_t base::get_addr() const {
      return addr;
    }

    void base::set_addr(const word_t addr) {
      this->addr = addr;
    }

    void base::set_section(const std::string section) {
      this->section = section;
    }

    std::string base::get_section() const {
      return section;
    }
  }
}
