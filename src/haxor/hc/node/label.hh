#ifndef HAXOR_HC_NODE_LABEL_HH
#define HAXOR_HC_NODE_LABEL_HH

#include "haxor/hc/node/base.hh"

namespace haxor {
  namespace node {
    class label : public base {
      public:
      label(const std::string &name);
      enum type get_type() const;
      std::string get_name();

      private:
      std::string name;
    };
  }
}

#endif
