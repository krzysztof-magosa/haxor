module Haxor
  module Vm
    module Cpu
    module Unit
      class Jumps < Base
        #         0x20
        OP_CALL = 0x20 # call a
        OP_JMP  = 0x21 # jmp a
        OP_JE   = 0x22 # je a
        OP_JG   = 0x23 # jg a
        OP_JGE  = 0x24 # jge a
        OP_JL   = 0x25 # jl a
        OP_JLE  = 0x26 # jle a
        OP_JNE  = 0x27 # jne a
        OP_JNG  = 0x28 # jng a
        OP_JNGE = 0x29 # jnge a
        OP_JNL  = 0x2a # jnl a
        OP_JNLE = 0x2b # jnle a
        OP_RET  = 0x2c # ret
        OP_IRET = 0x2d # iret
        #         0x3f

        def register
          bind_opcode OP_CALL,   :op_call
          bind_opcode OP_JMP,    :op_jmp
          bind_opcode OP_JE,     :op_je
          bind_opcode OP_JG,     :op_jg
          bind_opcode OP_JGE,    :op_jge
          bind_opcode OP_JL,     :op_jl
          bind_opcode OP_JLE,    :op_jle
          bind_opcode OP_JNE,    :op_jne
          bind_opcode OP_JNG,    :op_jng
          bind_opcode OP_JNGE,   :op_jnge
          bind_opcode OP_JNL,    :op_jnl
          bind_opcode OP_JNLE,   :op_jnle
          bind_opcode OP_RET,    :op_ret
          bind_opcode OP_IRET,   :op_iret
        end

        def fr_if(b)
          flags = fetch_cell 'fr'
          (flags & b) > 0
        end

        def op_call
          a = operand
          @vm.subsystem(:stack).push 'ip'
          jmp a
        end

        def op_jmp
          a = operand
          jmp a
        end

        def op_je
          a = operand
          jmp a if fr_if Consts::FR_ZERO
        end

        def op_jg
          a = operand
          jmp a if !fr_if(Consts::FR_ZERO) && !fr_if(Consts::FR_SIGN)
        end

        def op_jge
          a = operand
          jmp a if !fr_if(Consts::FR_SIGN) || fr_if(Consts::FR_ZERO)
        end

        def op_jl
          a = operand
          jmp a if !fr_if(Consts::FR_ZERO) && fr_if(Consts::FR_SIGN)
        end

        def op_jle
          a = operand
          jmp a if fr_if(Consts::FR_SIGN) || fr_if(Consts::FR_ZERO)
        end

        def op_jne
          a = operand
          jmp a unless fr_if Consts::FR_ZERO
        end

        def op_jng
          a = operand
          jmp a unless !fr_if(Consts::FR_ZERO) && !fr_if(Consts::FR_SIGN)
        end

        def op_jnge
          a = operand
          jmp a unless !fr_if(Consts::FR_SIGN) || fr_if(Consts::FR_ZERO)
        end

        def op_jnl
          a = operand
          jmp a unless !fr_if(Consts::FR_ZERO) && fr_if(Consts::FR_SIGN)
        end

        def op_jnle
          a = operand
          jmp a unless fr_if(Consts::FR_SIGN) || fr_if(Consts::FR_ZERO)
        end

        def op_ret
          @vm.subsystem(:stack).pop 'ip'
        end

        def op_iret
          @vm.subsystem(:stack).pop 'ip'
        end

        private

        def jmp(addr)
          @vm.subsystem(:mem).write 'ip', addr
        end
      end
    end
  end
end
end
