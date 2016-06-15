#ifndef HAXOR_HC_NODE_IMM_HH
#define HAXOR_HC_NODE_IMM_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  enum class imm_attr_t {
    lower_half,
    upper_half_lowered
  };

  class node_imm : public node_base {
    public:
    void set_attr(const imm_attr_t &attr);
    bool has_attr(const imm_attr_t &attr);

    private:
    std::vector<imm_attr_t> attrs;
  };
}

#endif
