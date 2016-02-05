#include "haxor/hc/node/num.hh"

namespace haxor {
  namespace node {
    num::num(const word_t value) {
      this->value = value;
      this->size = sizeof(word_t);
    }

    num::num(const word_t value, const size_t size) {
      this->value = value;
      this->size = size;
    }

    enum type num::get_type() const {
      return type::num;
    }

    word_t num::get_value() {
      return value;
    }
  }
}
