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
          bind_cmd 'shl', :cmd_shl
          bind_cmd 'shr', :cmd_shr
        end

        def cmd_and(a, b)
          add_cmd Vm::Cpu::Unit::Logical::OP_AND, a, b
        end

        def cmd_neg(a)
          add_cmd Vm::Cpu::Unit::Logical::OP_NEG, a
        end

        def cmd_not(a)
          add_cmd Vm::Cpu::Unit::Logical::OP_NOT, a
        end

        def cmd_or(a, b)
          add_cmd Vm::Cpu::Unit::Logical::OP_OR, a, b
        end

        def cmd_xor(a, b)
          add_cmd Vm::Cpu::Unit::Logical::OP_XOR, a, b
        end

        def cmd_shl(a, b)
          add_cmd Vm::Cpu::Unit::Logical::OP_SHL, a, b
        end

        def cmd_shr(a, b)
          add_cmd Vm::Cpu::Unit::Logical::OP_SHR, a, b
        end
      end
    end
  end
end
