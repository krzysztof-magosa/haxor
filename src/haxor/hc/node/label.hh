#ifndef HAXOR_HC_NODE_LABEL_HH
#define HAXOR_HC_NODE_LABEL_HH

#include "haxor/hc/node/imm.hh"

namespace haxor {
  class node_label : public node_imm {
    public:
    node_label(const std::string &name);
    node_type get_type() const;
    std::string get_name();

    node_label* clone();

    private:
    std::string name;
  };
}

#endif
