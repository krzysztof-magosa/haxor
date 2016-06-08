#include <stdexcept>
#include "haxor/hc/node/base.hh"

namespace haxor {
  std::string type_to_string(node_type nodeType) {
    switch (nodeType) {
    case node_type::program:
      return "PROGRAM";
    case node_type::instr:
      return "INSTRUCTION";
    case node_type::num:
      return "NUMBER";
    case node_type::reg:
      return "REGISTER";
    case node_type::label:
      return "LABEL";
    case node_type::section:
      return "SECTION";
    default:
      return "UNKNOWN";
    }
  }

  bool node_base::is_a(node_type type) {
    return (get_type() == type);
  }

  word_t node_base::get_size() const {
    return size;
  }

  word_t node_base::get_addr() const {
    return addr;
  }

  void node_base::set_addr(const word_t addr) {
    this->addr = addr;
  }

  void node_base::set_section(const std::string &section) {
    this->section = section;
  }

  std::string node_base::get_section() const {
    return section;
  }

  void node_base::set_location(const class location &location) {
    this->location = new class location(location);
  }

  bool node_base::has_location() {
    return this->location != nullptr;
  }

  class location& node_base::get_location() {
    if (!has_location()) {
      throw std::logic_error("This AST node does not have location.");
    }
    return *this->location;
  }
}

