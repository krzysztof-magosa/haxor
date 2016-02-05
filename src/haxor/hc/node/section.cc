#include "haxor/hc/node/section.hh"

namespace haxor {
  namespace node {
    section::section(const std::string &name) : name(name) {
    }

    enum type section::get_type() const {
      return type::section;
    }

    std::string section::get_name() const {
      return name;
    }
  }
}
