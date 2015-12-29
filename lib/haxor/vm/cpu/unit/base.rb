module Haxor
  module Vm
    module Cpu
    module Unit
      class Base
        attr_accessor :vm

        def register
          fail 'this method must be implemented.'
        end

        def bind_opcode(opcode, method)
          @vm.bind_opcode opcode, self, method
        end

        # TODO: BC
        def next_cell
          @vm.subsystem(:mem).next_word
        end

        # TODO: BC
        def fetch_cell(addr)
          @vm.subsystem(:mem).read addr
        end

        def dereference(addr)
          @vm.subsystem(:mem).read addr
        end

        # TODO: BC
        def replace_cell(addr, value)
          @vm.subsystem(:mem).write addr, value
        end

        def operand
          @vm.operand
        end

        def operands
          @vm.operands
        end
      end
    end
    end
  end
end
