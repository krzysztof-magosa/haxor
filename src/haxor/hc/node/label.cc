#include "haxor/hc/node/label.hh"

namespace haxor {
  node_label::node_label(const std::string &name) {
    this->name = name;
  }

  node_type node_label::get_type() const {
    return node_type::label;
  }

  std::string node_label::get_name() {
    return name;
  }

  node_label* node_label::clone() {
    return new node_label(*this);
  }
}
