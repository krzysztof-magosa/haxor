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
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_ADD, a, b
        end

        def cmd_sub(a, b)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_SUB, a, b
        end

        def cmd_div(a)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_DIV, a
        end

        def cmd_mul(a)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_MUL, a
        end

        def cmd_cmp(a, b)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_CMP, a, b
        end

        def cmd_inc(a)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_INC, a
        end

        def cmd_dec(a)
          add_cmd Vm::Cpu::Unit::Arithmetic::OP_DEC, a
        end
      end
    end
  end
end
