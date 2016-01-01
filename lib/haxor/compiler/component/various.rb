module Haxor
  module Compiler
    module Component
      class Various < Base
        def register
          bind_cmd 'nop', :cmd_nop
          bind_cmd 'lea', :cmd_lea
          bind_cmd 'int', :cmd_int
          bind_cmd 'syscall', :cmd_syscall
        end

        def cmd_nop(*_args)
          add_cmd Vm::Cpu::Unit::Various::OP_NOP
        end

        def cmd_lea(a, b)
          add_cmd Vm::Cpu::Unit::Various::OP_LEA, a, b
        end

        def cmd_int(a)
          add_cmd Vm::Cpu::Unit::Various::OP_INT, a
        end

        def cmd_syscall
          add_cmd Vm::Cpu::Unit::Various::OP_SYSCALL
        end
      end
    end
  end
end
