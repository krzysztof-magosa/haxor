#include "haxor/hc/node/imm.hh"

namespace haxor {
  void node_imm::set_attr(const imm_attr_t &attr) {
    attrs.push_back(attr);
  }

  bool node_imm::has_attr(const imm_attr_t &attr) {
    return std::find(attrs.begin(), attrs.end(), attr) != attrs.end();
  }
}
