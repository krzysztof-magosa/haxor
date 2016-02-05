#ifndef HAXOR_HC_NODE_INSTR_HH
#define HAXOR_HC_NODE_INSTR_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    class instr : public base {
      public:
      instr(const std::string &instr, std::vector<node::base*> *args);
      enum type get_type() const;
      std::string get_name() const;
      const std::vector<node::base*>* get_args() const;

      private:
      const std::string name;
      const std::vector<node::base*> *args;
    };
  }
}

#endif
