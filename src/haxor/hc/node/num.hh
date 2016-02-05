#ifndef HAXOR_HC_NODE_NUM_HH
#define HAXOR_HC_NODE_NUM_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    class num : public base {
      public:
      num(const word_t value);
      num(const word_t value, const size_t size);
      enum type get_type() const;
      word_t get_value();

      private:
      word_t value;
    };
  }
}

#endif
