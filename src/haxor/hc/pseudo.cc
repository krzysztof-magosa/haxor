#include <utility>
#include "haxor/hc/node/all.hh"
#include "haxor/hc/pseudo.hh"

namespace haxor {
  pseudo::pseudo() {
    ast = new std::vector<node_base*>();

    instr2func.insert(std::make_pair("push",  &pseudo::p_push));
    instr2func.insert(std::make_pair("pushi", &pseudo::p_pushi));
    instr2func.insert(std::make_pair("pushm", &pseudo::p_pushm));
    instr2func.insert(std::make_pair("pop",   &pseudo::p_pop));
    instr2func.insert(std::make_pair("popm",  &pseudo::p_popm));
    instr2func.insert(std::make_pair("move",  &pseudo::p_move));
    instr2func.insert(std::make_pair("clear", &pseudo::p_clear));
    instr2func.insert(std::make_pair("not",   &pseudo::p_not));
    instr2func.insert(std::make_pair("ret",   &pseudo::p_ret));
    instr2func.insert(std::make_pair("prol",  &pseudo::p_prol));
    instr2func.insert(std::make_pair("epil",  &pseudo::p_epil));
    instr2func.insert(std::make_pair("b",     &pseudo::p_b));
    instr2func.insert(std::make_pair("bal",   &pseudo::p_bal));
    instr2func.insert(std::make_pair("bgt",   &pseudo::p_bgt));
    instr2func.insert(std::make_pair("blt",   &pseudo::p_blt));
    instr2func.insert(std::make_pair("bge",   &pseudo::p_bge));
    instr2func.insert(std::make_pair("ble",   &pseudo::p_ble));
    instr2func.insert(std::make_pair("blez",  &pseudo::p_blez));
    instr2func.insert(std::make_pair("bgtz",  &pseudo::p_bgtz));
    instr2func.insert(std::make_pair("beqz",  &pseudo::p_beqz));
    instr2func.insert(std::make_pair("li",    &pseudo::p_li));
    instr2func.insert(std::make_pair("resw",  &pseudo::p_resw));
  }

  void pseudo::process(std::vector<node_base*> *input) {
    for (auto *item : *input) {
      if (item->get_type() == node_type::instr) {
        auto *instr = dynamic_cast<node_instr*>(item);
        auto it = instr2func.find(instr->get_name());

        if (it != instr2func.end()) {
          auto func = std::bind(it->second, this, instr);
          func();
        } else {
          ast->push_back(item);
        }
      } else {
        ast->push_back(item);
      }
    }
  }

  std::vector<node_base*> *pseudo::get_ast() {
    return ast;
  }

  void pseudo::p_push(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(-sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("sw", args));
    }
  }

  void pseudo::p_pushi(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("addi", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(-sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      args->push_back(new node_reg("$at"));
      ast->push_back(new node_instr("sw", args));
    }
  }

  void pseudo::p_pushm(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("lw", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(-sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      args->push_back(new node_reg("$at"));
      ast->push_back(new node_instr("sw", args));
    }
  }

  void pseudo::p_pop(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      ast->push_back(new node_instr("lw", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
  }

  void pseudo::p_popm(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      ast->push_back(new node_instr("lw", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      args->push_back(new node_reg("$at"));
      ast->push_back(new node_instr("sw", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
  }

  void pseudo::p_move(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      args->push_back(new node_reg("$zero"));
      ast->push_back(new node_instr("add", args));
    }
  }

  void pseudo::p_clear(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node_reg("$zero"));
      args->push_back(new node_reg("$zero"));
      ast->push_back(new node_instr("add", args));
    }
  }

  void pseudo::p_not(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      args->push_back(new node_reg("$zero"));
      ast->push_back(new node_instr("nor", args));
    }
  }

  void pseudo::p_ret(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$ra"));
      ast->push_back(new node_instr("jr", args));
    }
  }

  void pseudo::p_prol(node_instr *input) {
    make_push("$ra");
    make_push("$fp");
    make_push("$s0");
    make_push("$s1");
    make_push("$s2");
    make_push("$s3");
    make_push("$s4");
    make_push("$s5");
    make_push("$s6");
    make_push("$s7");
    make_push("$s8");
    make_push("$s9");

    // $fp = $sp
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$fp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      ast->push_back(new node_instr("addi", args));
    }
    // $sp -= imm
    {
      auto args = new std::vector<node_base*>();
      auto arg1 = dynamic_cast<node_num*>(input->get_args()->at(0));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(arg1->get_value()));
      ast->push_back(new node_instr("addi", args));
    }
  }

  void pseudo::p_epil(node_instr *input) {
    // $sp = $fp
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$fp"));
      args->push_back(new node_num(0));
      ast->push_back(new node_instr("addi", args));
    }

    make_pop("$s9");
    make_pop("$s8");
    make_pop("$s7");
    make_pop("$s6");
    make_pop("$s5");
    make_pop("$s4");
    make_pop("$s3");
    make_pop("$s2");
    make_pop("$s1");
    make_pop("$s0");
    make_pop("$fp");
    make_pop("$ra");

    // ret
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$ra"));
      ast->push_back(new node_instr("jr", args));
    }
  }

  void pseudo::p_b(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$zero"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("beq", args));
    }
  }

  void pseudo::p_bal(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$zero"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("beql", args));
    }
  }

  void pseudo::p_bgt(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(input->get_args()->at(1));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node_instr("bne", args));
    }
  }

  void pseudo::p_blt(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node_instr("bne", args));
    }
  }

  void pseudo::p_bge(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node_instr("beq", args));
    }
  }

  void pseudo::p_ble(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(input->get_args()->at(1));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(2));
      ast->push_back(new node_instr("beq", args));
    }
  }

  void pseudo::p_blez(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node_instr("beq", args));
    }
  }

  void pseudo::p_bgtz(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(0));
      ast->push_back(new node_instr("slt", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$at"));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node_instr("bne", args));
    }
  }

  void pseudo::p_beqz(node_instr *input) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(new node_reg("$zero"));
      args->push_back(input->get_args()->at(1));
      ast->push_back(new node_instr("beq", args));
    }
  }

  void pseudo::p_li(node_instr *input) {
    {
      auto *imm = dynamic_cast<node_imm*>(input->get_args()->at(1)->clone());
      imm->set_attr(imm_attr_t::upper_half_lowered);

      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(imm); // new node_num(num >> 32)
      ast->push_back(new node_instr("lui", args));
    }
    {
      auto *imm = dynamic_cast<node_imm*>(input->get_args()->at(1)->clone());
      imm->set_attr(imm_attr_t::lower_half);

      auto args = new std::vector<node_base*>();
      args->push_back(input->get_args()->at(0));
      args->push_back(input->get_args()->at(0));
      args->push_back(imm); //new node_num(num & 0xffffffff));
      ast->push_back(new node_instr("ori", args));
    }
  }

  void pseudo::p_resw(node_instr *input) {
    node_num *num = dynamic_cast<node_num*>(input->get_args()->at(0));

    for (word_t i = 0; i < num->get_value(); i++) {
      ast->push_back(new node_num(0));
    }
  }

  void pseudo::make_push(const std::string &reg) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(- sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      args->push_back(new node_reg(reg));
      ast->push_back(new node_instr("sw", args));
    }
  }

  void pseudo::make_pop(const std::string &reg) {
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg(reg));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(0));
      ast->push_back(new node_instr("lw", args));
    }
    {
      auto args = new std::vector<node_base*>();
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_reg("$sp"));
      args->push_back(new node_num(sizeof(word_t)));
      ast->push_back(new node_instr("addi", args));
    }
  }
}
