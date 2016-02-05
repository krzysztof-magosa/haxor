#include "haxor/hc/node/instr.hh"

namespace haxor {
  namespace node {
    instr::instr(const std::string &name, std::vector<node::base*> *args) : name(name), args(args) {
      this->size = sizeof(word_t);
    }

    enum type instr::get_type() const {
      return type::instr;
    }

    std::string instr::get_name() const {
      return name;
    }

    const std::vector<node::base*>* instr::get_args() const {
      return args;
    }
  }
}
