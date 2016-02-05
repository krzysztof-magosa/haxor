#ifndef HAXOR_HC_PSEUDO_HH
#define HAXOR_HC_PSEUDO_HH

#include <map>
#include <functional>
#include "haxor/hc/node/all.hh"

namespace haxor {
  class pseudo {
    public:
    pseudo();
    void process(std::vector<node::base*> *input);
    std::vector<node::base*> *get_ast();

    private:
    void p_push(node::instr *input);
    void p_pushi(node::instr *input);
    void p_pushm(node::instr *input);
    void p_pop(node::instr *input);
    void p_popm(node::instr *input);
    void p_move(node::instr *input);
    void p_clear(node::instr *input);
    void p_not(node::instr *input);
    void p_ret(node::instr *input);
    void p_prol(node::instr *input);
    void p_epil(node::instr *input);
    void p_b(node::instr *input);
    void p_bal(node::instr *input);
    void p_bgt(node::instr *input);
    void p_blt(node::instr *input);
    void p_bge(node::instr *input);
    void p_ble(node::instr *input);
    void p_blez(node::instr *input);
    void p_bgtz(node::instr *input);
    void p_beqz(node::instr *input);

    std::vector<node::base*> *ast;

    typedef void (pseudo::*pseudo_member)(node::instr*);
    std::map<std::string, pseudo_member> instr2func;
  };
}

#endif
