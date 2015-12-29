module Haxor
  module Vm
    module Cpu
    module Unit
      class Transfer < Base
        #            0x60
        OP_MOV     = 0x60 # mov a, b
        OP_PUSH    = 0x61 # push a
        OP_POP     = 0x62 # pop a
        #            0x7f

        def register
          bind_opcode OP_MOV,     :op_mov
          bind_opcode OP_PUSH,    :op_push
          bind_opcode OP_POP,     :op_pop
        end

        def op_mov
          a, b = operands
          v = @vm.subsystem(:mem).read b
          @vm.subsystem(:mem).write a, v
        end

        def op_push
          a = operand
          @vm.subsystem(:stack).push a
        end

        def op_pop
          a = operand
          @vm.subsystem(:stack).pop a
        end
      end
    end
  end
end
end
