#ifndef HAXOR_HC_NODE_REG_HH
#define HAXOR_HC_NODE_REG_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  class node_reg : public node_base {
    public:
    node_reg(const std::string &name);
    node_type get_type() const;
    std::string get_name() const;

    private:
    const std::string name;
  };
}

#endif
