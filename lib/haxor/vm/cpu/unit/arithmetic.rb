module Haxor
  module Vm
    module Cpu
    module Unit
      class Arithmetic < Base
        #            0x00 (not used)
        OP_ADD     = 0x01 # add a, b
        OP_SUB     = 0x02 # sub a, b
        OP_DIV     = 0x03 # div a
        OP_MUL     = 0x04 # mul a
        OP_INC     = 0x05 # inc a
        OP_DEC     = 0x06 # dec a
        OP_CMP     = 0x07 # cmp a, b
        #            0x1f

        def register
          bind_opcode OP_ADD, :op_add
          bind_opcode OP_SUB, :op_sub
          bind_opcode OP_DIV, :op_div
          bind_opcode OP_MUL, :op_mul
          bind_opcode OP_INC, :op_inc
          bind_opcode OP_DEC, :op_dec
          bind_opcode OP_CMP, :op_cmp
        end

        def op_add
          a, b = operands
          c = @vm.subsystem(:mem).read(a) + @vm.subsystem(:mem).read(b)
          @vm.subsystem(:mem).write a, c
        end

        def op_sub
          a, b = operands
          c = @vm.subsystem(:mem).read(a) - @vm.subsystem(:mem).read(b)
          @vm.subsystem(:mem).write a, c
        end

        def op_div
          a = operand
          av = @vm.subsystem(:mem).read a
          arv = @vm.subsystem(:mem).read 'ar'
          @vm.subsystem(:mem).write 'ar', (arv / av)
          @vm.subsystem(:mem).write 'dr', (arv % av)
        end

        def op_mul
          a = operand
          av = @vm.subsystem(:mem).read a
          arv = @vm.subsystem(:mem).read 'ar'
          @vm.subsystem(:mem).write 'ar', (arv * av)
        end

        def op_inc
          a = operand
          av = @vm.subsystem(:mem).read a
          @vm.subsystem(:mem).write a, (av + 1)
        end

        def op_dec
          a = operand
          av = @vm.subsystem(:mem).read a
          @vm.subsystem(:mem).write a, (av - 1)
        end

        def op_cmp
          a, b = operands
          av = @vm.subsystem(:mem).read a
          bv = @vm.subsystem(:mem).read b
          v = av - bv
          fr_set Consts::FR_ZERO, (v == 0)
          fr_set Consts::FR_SIGN, v < 0
        end

        def fr_set(b, enable)
          flags = fetch_cell 'fr'
          if enable
            flags |= b
          else
            flags &= ~b
          end
          replace_cell 'fr', flags
        end

        def fr_if(b)
          flags = fetch_cell 'fr'
          (flags & b) > 0
        end
      end
    end
    end
  end
end
