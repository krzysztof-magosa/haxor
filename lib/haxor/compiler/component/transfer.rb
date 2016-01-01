module Haxor
  module Compiler
    module Component
      class Transfer < Base
        def register
          bind_cmd 'mov', :cmd_mov
          bind_cmd 'push', :cmd_push
          bind_cmd 'pop', :cmd_pop
        end

        def cmd_push(a)
          add_cmd Vm::Cpu::Unit::Transfer::OP_PUSH, a
        end

        def cmd_pop(a)
          add_cmd Vm::Cpu::Unit::Transfer::OP_POP, a
        end

        def cmd_mov(a, b)
          add_cmd Vm::Cpu::Unit::Transfer::OP_MOV, a, b
        end
      end
    end
  end
end
