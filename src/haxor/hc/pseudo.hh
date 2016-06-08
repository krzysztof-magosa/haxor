#ifndef HAXOR_HC_PSEUDO_HH
#define HAXOR_HC_PSEUDO_HH

#include <map>
#include <functional>
#include "haxor/hc/node/all.hh"

namespace haxor {
  class pseudo {
    public:
    pseudo();
    void process(std::vector<node_base*> *input);
    std::vector<node_base*> *get_ast();

    private:
    void p_push(node_instr *input);
    void p_pushi(node_instr *input);
    void p_pushm(node_instr *input);
    void p_pop(node_instr *input);
    void p_popm(node_instr *input);
    void p_move(node_instr *input);
    void p_clear(node_instr *input);
    void p_not(node_instr *input);
    void p_ret(node_instr *input);
    void p_prol(node_instr *input);
    void p_epil(node_instr *input);
    void p_b(node_instr *input);
    void p_bal(node_instr *input);
    void p_bgt(node_instr *input);
    void p_blt(node_instr *input);
    void p_bge(node_instr *input);
    void p_ble(node_instr *input);
    void p_blez(node_instr *input);
    void p_bgtz(node_instr *input);
    void p_beqz(node_instr *input);

    std::vector<node_base*> *ast;

    typedef void (pseudo::*pseudo_member)(node_instr*);
    std::map<std::string, pseudo_member> instr2func;
  };
}

#endif
