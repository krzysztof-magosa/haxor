#ifndef HAXOR_HC_NODE_LABEL_HH
#define HAXOR_HC_NODE_LABEL_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  class node_label : public node_base {
    public:
    node_label(const std::string &name);
    enum node_type get_type() const;
    std::string get_name();

    private:
    std::string name;
  };
}

#endif
