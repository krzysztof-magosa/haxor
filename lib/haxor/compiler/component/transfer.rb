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
          add Token::Cmd.new(Vm::Cpu::Unit::Transfer::OP_PUSH | offset_flags(a))
          parse_value a
        end

        def cmd_pop(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Transfer::OP_POP | offset_flags(a))
          parse_value a
        end

        def cmd_mov(a, b)
          add Token::Cmd.new(Vm::Cpu::Unit::Transfer::OP_MOV | offset_flags(a, b))
          parse_value a
          parse_value b
        end
      end
    end
  end
end
