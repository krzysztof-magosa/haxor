module Haxor
  module Vm
    module Cpu
      module Unit
        class Various < Base
          #            = 0x80
          OP_LEA       = 0x80 # lea a
          OP_NOP       = 0x81 # nop
          OP_INT       = 0x85 # int a
          OP_SYSCALL   = 0x86 # syscall
          #              0x9f

          def register
            bind_opcode OP_LEA,      :op_lea
            bind_opcode OP_NOP,      :op_nop
            bind_opcode OP_INT,      :op_int
            bind_opcode OP_SYSCALL,  :op_syscall
          end

          def op_lea
            a, b = operands
            @vm.subsystem(:mem).write a, b
          end

          def op_nop
            # intentionally nothing here
          end

          def op_int
            a = operand

            @vm.subsystem(:stack).push 'ip'

            av = @vm.subsystem(:mem).read a
            ivt = Consts::IVT_ADDR + (av * 8)
            handler = @vm.subsystem(:mem).read ivt
            @vm.subsystem(:mem).write 'ip', handler
          end

          def op_syscall
            @vm.subsystem(:os).syscall
          end
        end
      end
    end
  end
end
