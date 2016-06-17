#include "haxor/hc/node/all.hh"
#include "haxor/hc/compiler.hh"
#include "haxor/hc/pseudo.hh"
#include "haxor/common/hdr.hh"
#include "haxor/common/opcode.hh"
#include <sstream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace haxor {
  compiler::compiler() : lexer(*this), parser(lexer, *this) {
  }

  void compiler::ensure_its_register(node_base * const arg) {
    if (!arg->is_a(node_type::reg)) {
      std::string error_msg = "Invalid argument, unexpected "
        + type_to_string(arg->get_type())
        + ", expecting "
        + type_to_string(node_type::reg);
      if (arg->has_location()) {
        error_msg += " at " + format_location(arg->get_location());
      }
      throw hc_syntax_error(error_msg);
    }
  }

  void compiler::ensure_its_immediate(node_base * const arg) {
    if (!arg->is_a(node_type::num) && !arg->is_a(node_type::label)) {
      std::string error_msg = "Invalid argument, unexpected "
        + type_to_string(arg->get_type())
        + ", expcting "
        + type_to_string(node_type::num)
        + " or "
        + type_to_string(node_type::label);
      if (arg->has_location()) {
        error_msg += ", at " + format_location(arg->get_location());
      }
      throw hc_syntax_error(error_msg);
    }
  }

  void compiler::ensure_its_num(node_base * const arg) {
    if (!arg->is_a(node_type::num)) {
      std::string error_msg = "Invalid argument, unexpected "
        + type_to_string(arg->get_type())
        + ", expcting "
        + type_to_string(node_type::num);
      if (arg->has_location()) {
        error_msg += ", at " + format_location(arg->get_location());
      }
      throw hc_syntax_error(error_msg);
    }
  }

  void compiler::verify_code() {
    for (auto *item : *ast) {
      if (item->is_a(node_type::instr)) {
        auto *instr = dynamic_cast<node_instr*>(item);

        auto it = instr_list.find(instr->get_name());

        if (it != instr_list.end()) {
          auto const &instr_def = it->second;

          if (instr->get_args()->size() != instr_def.args.size()) {
            throw std::runtime_error("Invalid number of arguments.");
          }

          for (size_t i = 0; i < instr_def.args.size(); i++) {
            auto * const arg = instr->get_args()->at(i);

            if (instr_def.args[i] == 'r') {
              ensure_its_register(arg);
            } else if (instr_def.args[i] == 'i') {
              ensure_its_immediate(arg);
            } else if (instr_def.args[i] == 'n') {
              ensure_its_num(arg);
            }
          }
        } else {
          std::string error_msg = "Unknown instruction '" + instr->get_name() + "'";
          if (instr->has_location()) {
            error_msg += ", at location " + format_location(instr->get_location());
          }
          throw hc_syntax_error(error_msg); // set correct exception type
        }
      } else if (item->is_a(node_type::section)) {
        auto *section = dynamic_cast<node_section*>(item);

        if (std::find(sections.begin(), sections.end(), section->get_name()) == sections.end()) {
          std::string error_msg = "Unsupported section '" + section->get_name() + "'";
          if (section->has_location()) {
            error_msg += ", at location " + format_location(section->get_location());
          }
          throw hc_syntax_error(error_msg);
        }
      }
    }
  }

  void compiler::verify_labels () {
    for (auto item : *ast) {
      if (item->is_a(node_type::instr)) {
        auto *instr = dynamic_cast<node_instr*>(item);

        for (auto *arg : *instr->get_args()) {
          if (arg->is_a(node_type::label)) {
            auto *label = dynamic_cast<node_label*>(arg);

            if (labels.find(label->get_name()) == labels.end()) {
              std::string error_msg = "Reference to not existing label '" + label->get_name() + "'";
              if (label->has_location()) {
                error_msg += ", at location " + format_location(label->get_location());
              }

              throw hc_syntax_error(error_msg);
            }
          }
        }
      }
    }
  }

  void compiler::unwind_pseudo() {
    pseudo pseudo;
    pseudo.process(ast);
    delete ast;
    ast = pseudo.get_ast();
  }

  void compiler::mark_sections() {
    std::string current_section = section_text;
    for (auto *item : *ast) {
      if (item->get_type() == node_type::section) {
        current_section = dynamic_cast<node_section*>(item)->get_name();
      }
      item->set_section(current_section);
    }
  }

  void compiler::order_sections() {
    auto *sorted = new std::vector<node_base*>();
    for (auto section : sections) {
      std::copy_if(
        ast->begin(), ast->end(), std::back_inserter(*sorted),
        [section](node_base *n) {
          return n->get_section() == section;
        }
      );
    }
    delete ast;
    ast = sorted;
  }

  void compiler::calc_addresses() {
    int64_t current_address = ivt_size;

    for (auto *item : *ast) {
      item->set_addr(current_address);
      current_address += item->get_size();
    }
  }

  void compiler::collect_labels() {
    labels.clear();
    for (auto *item : *ast) {
      if (item->is_a(node_type::label)) {
        auto *label = dynamic_cast<node_label*>(item);
        labels.insert(std::make_pair(label->get_name(), label->get_addr()));
      }
    }
  }

  int64_t compiler::calc_section_size(const std::string &name) {
    int64_t size = 0;
    for (auto *item : *ast) {
      if (item->get_section() == name) {
        size += item->get_size();
      }
    }

    return size;
  }

  hdr_t compiler::build_hdr() {
    hdr_t hdr;
    hdr.magic = magic;
    hdr.version = version;

    if (labels.find("main") == labels.end()) {
      throw hc_missing_entry_point();
    }

    hdr.entry_point = labels.at("main");
    hdr.text_size = calc_section_size(section_text);
    hdr.data_size = calc_section_size(section_data);
    hdr.bss_size = calc_section_size(section_bss);
    hdr.stack_size = 4096 * sizeof(word_t); // @TODO add option to customize

    return hdr;
  }

  std::string& compiler::get_filename() {
    return filename;
  }

  void compiler::compile(const std::string &filename) {
    this->filename = filename;

    std::ifstream source;
    source.open(filename);

    if (source.fail()) {
      throw std::runtime_error("Cannot open source file: " + filename + ".");
    }

    lexer.set_stream(source);

    parser.parse();

    verify_code();
    unwind_pseudo();
    mark_sections();
    order_sections();
    calc_addresses();
    collect_labels();
    verify_labels();

    auto hdr = build_hdr();

    // compile
    std::fstream haxe;
    haxe.open(filename + ".e", std::ios::binary | std::fstream::out);
    haxe.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));

    for (auto item : *ast) {
      if (item->get_section() == ".bss") {
        continue;
      }

      if (item->is_a(node_type::instr)) {
        node_instr *instr = dynamic_cast<node_instr*>(item);
        opcode_t opcode;
        opcode.cmd = instr_list.at(instr->get_name()).cmd;

        int reg_no = 1;
        for (auto arg : *instr->get_args()) {
          if (arg->is_a(node_type::reg)) {
            auto reg = resolve_reg(dynamic_cast<node_reg*>(arg)->get_name());

            opcode.set_reg(reg_no, reg);
            reg_no ++;
          } else if (arg->is_a(node_type::num) || arg->is_a(node_type::label)) {
            node_imm *arg_imm = dynamic_cast<node_imm*>(arg);
            word_t imm;

            if (arg->is_a(node_type::num)) {
              imm = dynamic_cast<node_num*>(arg)->get_value();
            } else if (arg->is_a(node_type::label)) {
              imm = labels.at(dynamic_cast<node_label*>(arg)->get_name());
            }

            if (arg_imm->has_attr(imm_attr_t::upper_half_lowered)) {
              imm = imm >> 32;
            }

            if (arg_imm->has_attr(imm_attr_t::lower_half)) {
              imm &= 0xffffffff;
            }

            if (instr_list.at(instr->get_name()).rel) {
              imm -= (item->get_addr() + sizeof(word_t));
            }

            if (instr_list.at(instr->get_name()).x8) {
              imm /= sizeof(word_t);
            }

             opcode.imm = imm;
           } else {
             std::cout << "fail" << std::endl;
           }
         }

         auto value = encode_opcode(opcode);
         haxe.write(reinterpret_cast<char*>(&value), sizeof(value));
      } else if (item->is_a(node_type::num)) {
        word_t value = dynamic_cast<node_num*>(item)->get_value();
        haxe.write(reinterpret_cast<char*>(&value), item->get_size());
      }
    }
  }

  uint8_t compiler::resolve_reg(const std::string &name) {
    auto item = reg_aliases.find(name);

    if (item != reg_aliases.end()) {
      return item->second;
    } else {
      return std::stoi(name.substr(1, std::string::npos));
    }

    // @TODO validation
  }

  void compiler::set_ast(std::vector<node_base*> *ast) {
    this->ast = ast;
  }

  std::string compiler::format_location(const class location &location) {
    std::stringstream ss;
    ss << location;
    return ss.str();
  }
}
