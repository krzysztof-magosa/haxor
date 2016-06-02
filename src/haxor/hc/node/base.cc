#include <stdexcept>
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

    void base::set_section(const std::string &section) {
      this->section = section;
    }

    std::string base::get_section() const {
      return section;
    }

    void base::set_location(const class location &location) {
      this->location = new class location(location);
    }

    bool base::has_location() {
      return this->location != nullptr;
    }

    class location& base::get_location() {
      if (!has_location()) {
        throw std::logic_error("This AST node does not have location.");
      }
      return *this->location;
    }
  }
}
