#include "haxor/hc/node/label.hh"

namespace haxor {
  namespace node {
    label::label(const std::string &name) {
      this->name = name;
    }

    enum type label::get_type() const {
      return type::label;
    }

    std::string label::get_name() {
      return name;
    }
  }
}
