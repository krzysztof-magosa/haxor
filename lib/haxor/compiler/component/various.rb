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
          add Token::Cmd.new(Vm::Cpu::Unit::Various::OP_NOP)
        end

        def cmd_lea(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Various::OP_LEA | offset_flags(a, b))
          parse_value a
          parse_value b
        end

        def cmd_int(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Various::OP_INT)
          parse_value a
        end

        def cmd_syscall
          add Token::Cmd.new(Vm::Cpu::Unit::Various::OP_SYSCALL)
        end
      end
    end
  end
end
