module Haxor
  module Compiler
    module Component
      class Logical < Base
        def register
          bind_cmd 'and', :cmd_and
          bind_cmd 'neg', :cmd_neg
          bind_cmd 'not', :cmd_not
          bind_cmd 'or',  :cmd_or
          bind_cmd 'xor', :cmd_xor
        end

        def cmd_and(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Logical::OP_AND | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_neg(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Logical::OP_NEG | offset_flags(a))
          parse_value a
        end

        def cmd_not(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Logical::OP_NOT | offset_flags(a))
          parse_value a
        end

        def cmd_or(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Logical::OP_OR | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_xor(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Logical::OP_XOR | offset_flags(a, b))
          parse_value a
          parse_value b
        end
      end
    end
  end
end
