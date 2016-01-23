#include "haxor/hvm/haxor.hh"
#include "haxor/hvm/regs.hh"

namespace haxor {
  regs::regs() {
    data.resize(64, 0);
  }

  word_t regs::read(const size_t id) const {
    validate_id(id);
    return data[id];
  }

  void regs::write(const size_t id, const word_t value) {
    validate_id(id);
    data[id] = value;
  }

  word_t regs::add(const size_t id, const word_t value) {
    validate_id(id);
    data[id] += value;
    return data[id];
  }

  void regs::validate_id(const size_t id) const {
    if (id >= data.size()) {
      throw regs_range_error();
    }
  }
}
