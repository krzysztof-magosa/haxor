#include "haxor/hc/node/reg.hh"

namespace haxor {
  namespace node {
    reg::reg(const std::string &name) : name(name) {
    }

    enum type reg::get_type() const {
      return type::reg;
    }

    std::string reg::get_name() const {
      return name;
    }
  }
}

