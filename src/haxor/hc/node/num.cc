#include "haxor/hc/node/num.hh"

namespace haxor {
  node_num::node_num(const word_t value) {
    this->value = value;
    this->size = sizeof(word_t);
  }

  node_num::node_num(const word_t value, const size_t size) {
    this->value = value;
    this->size = size;
  }

  enum node_type node_num::get_type() const {
    return node_type::num;
  }

  word_t node_num::get_value() {
    return value;
  }
}
