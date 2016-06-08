#ifndef HAXOR_HC_NODE_NUM_HH
#define HAXOR_HC_NODE_NUM_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  class node_num : public node_base {
    public:
    node_num(const word_t value);
    node_num(const word_t value, const size_t size);
    node_type get_type() const;
    word_t get_value();

    private:
    word_t value;
  };
}

#endif
