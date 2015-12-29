module Haxor
  module Vm
    module Cpu
    module Unit
      class Logical < Base
        #            0x40
        OP_AND     = 0x40 # and a, b
        OP_NEG     = 0x41 # neg a
        OP_NOT     = 0x42 # not a
        OP_OR      = 0x43 # or a, b
        OP_XOR     = 0x44 # xor a, b
        #            0x5f

        def register
          bind_opcode OP_AND,     :op_and
          bind_opcode OP_NEG,     :op_neg
          bind_opcode OP_NOT,     :op_not
          bind_opcode OP_OR,      :op_or
          bind_opcode OP_XOR,     :op_xor
        end

        def op_and
          a, b = operands
          av = @vm.subsystem(:mem).read a
          bv = @vm.subsystem(:mem).read b
          v = av & bv
          @vm.subsystem(:mem).write a, v
        end

        def op_neg
          a = operand
          av = @vm.subsystem(:mem).read a
          @vm.subsystem(:mem).write a, -av
        end

        def op_not
          a = operand
          av = @vm.subsystem(:mem).read a
          @vm.subsystem(:mem).write a, ~av
        end

        def op_or
          a, b = operands
          av = @vm.subsystem(:mem).read a
          bv = @vm.subsystem(:mem).read b
          @vm.subsystem(:mem).write a, (av | bv)
        end

        def op_xor
          a, b = operands
          av = @vm.subsystem(:mem).read a
          bv = @vm.subsystem(:mem).read b
          @vm.subsystem(:mem).write a, (av ^ bv)
        end
      end
    end
  end
end
end
