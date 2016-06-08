#include "haxor/hc/node/reg.hh"

namespace haxor {
  node_reg::node_reg(const std::string &name) : name(name) {
  }

  enum node_type node_reg::get_type() const {
    return node_type::reg;
  }

  std::string node_reg::get_name() const {
    return name;
  }
}

