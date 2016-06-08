#include "haxor/hc/node/section.hh"

namespace haxor {
  node_section::node_section(const std::string &name) : name(name) {
  }

  node_type node_section::get_type() const {
    return node_type::section;
  }

  std::string node_section::get_name() const {
    return name;
  }
}
