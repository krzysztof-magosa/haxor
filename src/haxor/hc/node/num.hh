#ifndef HAXOR_HC_NODE_NUM_HH
#define HAXOR_HC_NODE_NUM_HH

#include "haxor/hc/node/imm.hh"

namespace haxor {
  class node_num : public node_imm {
    public:
    node_num(const word_t value);
    node_num(const word_t value, const size_t size);
    node_type get_type() const;
    word_t get_value();

    node_num* clone();

    private:
    word_t value;
  };
}

#endif
