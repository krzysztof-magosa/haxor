#ifndef HAXOR_HC_NODE_SECTION_HH
#define HAXOR_HC_NODE_SECTION_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  class node_section : public node_base {
    public:
    node_section(const std::string &name);
    node_type get_type() const;
    std::string get_name() const;

    private:
    const std::string name;
  };
}

#endif
