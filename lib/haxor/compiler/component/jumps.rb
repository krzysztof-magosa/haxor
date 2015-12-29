module Haxor
  module Compiler
    module Component
      class Jumps < Base
        def register
          bind_cmd 'call', :cmd_call
          bind_cmd 'ret', :cmd_ret
          bind_cmd 'iret', :cmd_iret

          bind_cmd 'jmp', :cmd_jmp

          bind_cmd 'je',  :cmd_je
          bind_cmd 'jg',  :cmd_jg
          bind_cmd 'jge', :cmd_jge
          bind_cmd 'jl',  :cmd_jl
          bind_cmd 'jle', :cmd_jle

          bind_cmd 'jne',  :cmd_jne
          bind_cmd 'jng',  :cmd_jng
          bind_cmd 'jnge', :cmd_jnge
          bind_cmd 'jnl',  :cmd_jnl
          bind_cmd 'jnle', :cmd_jnle
        end

        def cmd_call(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_CALL | offset_flags(a))
          parse_value a
        end

        def cmd_ret(*_args)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_RET)
        end

        def cmd_iret(*_args)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_IRET)
        end

        def cmd_jmp(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JMP | offset_flags(a))
          parse_value a
        end

        def cmd_je(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JE)
          parse_value a
        end

        def cmd_jg(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JG)
          parse_value a
        end

        def cmd_jge(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JGE)
          parse_value a
        end

        def cmd_jl(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JL)
          parse_value a
        end

        def cmd_jle(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JLE)
          parse_value a
        end

        def cmd_jne(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JNE)
          parse_value a
        end

        def cmd_jng(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JNG)
          parse_value a
        end

        def cmd_jnge(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JNGE)
          parse_value a
        end

        def cmd_jnl(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JNL)
          parse_value a
        end

        def cmd_jnle(a)
          add Token::Cmd.new(Vm::Cpu::Unit::Jumps::OP_JNLE)
          parse_value a
        end
      end
    end
  end
end
