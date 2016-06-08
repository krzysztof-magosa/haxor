#include "haxor/hc/node/instr.hh"

namespace haxor {
  node_instr::node_instr(const std::string &name, std::vector<node_base*> *args) : name(name), args(args) {
    this->size = sizeof(word_t);
  }

  enum node_type node_instr::get_type() const {
    return node_type::instr;
  }

  std::string node_instr::get_name() const {
    return name;
  }

  const std::vector<node_base*>* node_instr::get_args() const {
    return args;
  }
}
