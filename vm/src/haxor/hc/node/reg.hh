#ifndef HAXOR_HC_NODE_REG_HH
#define HAXOR_HC_NODE_REG_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    class reg : public base {
      public:
      reg(const std::string &name);
      enum type get_type() const;
      std::string get_name() const;

      private:
      const std::string name;
    };
  }
}

#endif
