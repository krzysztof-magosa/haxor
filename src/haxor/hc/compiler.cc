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

  void compiler::verify_code() {
    for (auto *item : *ast) {
      if (item->is_a(node::type::instr)) {
        auto *instr = dynamic_cast<node::instr*>(item);

        auto it = instr_list.find(instr->get_name());

        if (it != instr_list.end()) {
          auto const &instr_def = it->second;

          if (instr->get_args()->size() != instr_def.args.size()) {
            throw std::runtime_error("Invalid number of arguments.");
          }

          for (size_t i = 0; i < instr_def.args.size(); i++) {
            auto * const arg = instr->get_args()->at(i);

            if (instr_def.args[i] == 'r') {
              if (!arg->is_a(node::type::reg)) {
                std::string error_msg = "Invalid argument, expected register";
                if (arg->has_location()) {
                  error_msg += ", at location " + format_location(arg->get_location());
                }
                throw hc_syntax_error(error_msg);
              }
            } else if (instr_def.args[i] == 'i') {
              if (!arg->is_a(node::type::num) && !arg->is_a(node::type::label)) {
                std::string error_msg = "Invalid argument, expected number or label";
                if (arg->has_location()) {
                  error_msg += ", at location " + format_location(arg->get_location());
                }
                throw std::runtime_error(error_msg);
              }
            }
          }
        } else {
          std::string error_msg = "Unknown instruction '" + instr->get_name() + "'";
          if (instr->has_location()) {
            error_msg += ", at location " + format_location(instr->get_location());
          }
          throw hc_syntax_error(error_msg); // set correct exception type
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
    std::string current_section = ".text";
    for (auto *item : *ast) {
      if (item->get_type() == node::type::section) {
        current_section = dynamic_cast<node::section*>(item)->get_name();
      }
      item->set_section(current_section);
    }
  }

  void compiler::order_sections() {
    std::vector<std::string> sections = { ".text", ".data" };
    for (auto *item : *ast) {
      if (item->get_section() == ".bss") {
        continue;
      }

      if (std::find(sections.begin(), sections.end(), item->get_section()) == sections.end()) {
        sections.push_back(item->get_section());
      }
    }
    sections.push_back(".bss"); // bss must be always last one

    auto *sorted = new std::vector<node::base*>();
    for (auto section : sections) {
      std::copy_if(
        ast->begin(), ast->end(), std::back_inserter(*sorted),
        [section](node::base *n) {
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
      if (item->is_a(node::type::label)) {
        auto *label = dynamic_cast<node::label*>(item);
        labels.insert(std::make_pair(label->get_name(), label->get_addr()));
      }
    }
  }

  int64_t compiler::calc_bss_size() {
    int64_t bss_size = 0;
    for (auto *item : *ast) {
      if (item->get_section() == ".bss") {
        bss_size += item->get_size();
      }
    }

    return bss_size;
  }

  hdr_t compiler::build_hdr() {
    hdr_t hdr;
    hdr.magic = magic;
    hdr.version = version;

    if (labels.find("main") == labels.end()) {
      throw hc_missing_entry_point();
    }

    hdr.entry_point = labels.at("main");
    hdr.stack_size = 4096 * sizeof(word_t); // @TODO add option to customize
    hdr.bss_size = calc_bss_size();

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

    auto hdr = build_hdr();

    // compile
    std::fstream haxe;
    haxe.open(filename + ".e", std::ios::binary | std::fstream::out);
    haxe.write(reinterpret_cast<char*>(&hdr), sizeof(hdr));

    for (auto item : *ast) {
      if (item->get_section() == ".bss") {
        continue;
      }

      if (item->is_a(node::type::instr)) {
        node::instr *instr = dynamic_cast<node::instr*>(item);
        opcode_t opcode;
        opcode.cmd = instr_list.at(instr->get_name()).cmd;

        int reg_no = 1;
        for (auto arg : *instr->get_args()) {
          if (arg->is_a(node::type::reg)) {
            auto reg = resolve_reg(dynamic_cast<node::reg*>(arg)->get_name());

            opcode.set_reg(reg_no, reg);
            reg_no ++;
          } else if (arg->is_a(node::type::num)) {
            opcode.imm = dynamic_cast<node::num*>(arg)->get_value();
          } else if (arg->get_type() == node::type::label) {
            word_t imm = labels.at(dynamic_cast<node::label*>(arg)->get_name());

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
      } else if (item->is_a(node::type::num)) {
        word_t value = dynamic_cast<node::num*>(item)->get_value();
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

  void compiler::set_ast(std::vector<node::base*> *ast) {
    this->ast = ast;
  }

  std::string compiler::format_location(const class location &location) {
    std::stringstream ss;
    ss << location;
    return ss.str();
  }
}
