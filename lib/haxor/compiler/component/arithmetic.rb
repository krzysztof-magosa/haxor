module Haxor
  module Compiler
    module Component
      class Arithmetic < Base
        def register
          bind_cmd 'inc', :cmd_inc
          bind_cmd 'dec', :cmd_dec
          bind_cmd 'add', :cmd_add
          bind_cmd 'sub', :cmd_sub
          bind_cmd 'div', :cmd_div
          bind_cmd 'mul', :cmd_mul
          bind_cmd 'cmp', :cmd_cmp
        end

        def cmd_add(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_ADD | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_sub(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_SUB | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_div(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_DIV | offset_flags(a))
          parse_value a
        end

        def cmd_mul(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_MUL | offset_flags(a))
          parse_value a
        end

        def cmd_cmp(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_CMP | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_inc(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_INC | offset_flags(a))
          parse_value a
        end

        def cmd_dec(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Arithmetic::OP_DEC | offset_flags(a))
          parse_value a
        end
      end
    end
  end
end
