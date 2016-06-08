#ifndef HAXOR_HC_NODE_INSTR_HH
#define HAXOR_HC_NODE_INSTR_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  class node_instr : public node_base {
    public:
    node_instr(const std::string &instr, std::vector<node_base*> *args);
    node_type get_type() const;
    std::string get_name() const;
    const std::vector<node_base*>* get_args() const;

    private:
    const std::string name;
    const std::vector<node_base*> *args;
  };
}

#endif
