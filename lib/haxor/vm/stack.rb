module Haxor
  module Vm
    class Stack < Subsystem
      def push(addr)
        value = @vm.subsystem(:mem).read addr
        push_value value
      end

      def push_value(value)
        sp = @vm.subsystem(:registers).read 'sp'
        sp -= Consts::WORD_SIZE
        @vm.subsystem(:registers).write 'sp', sp
        @vm.subsystem(:mem).write sp, value
      end

      def pop(addr)
        value = pop_value
        @vm.subsystem(:mem).write addr, value
      end

      def pop_value
        sp = @vm.subsystem(:registers).read 'sp'
        value = @vm.subsystem(:mem).read sp
        sp += Consts::WORD_SIZE
        @vm.subsystem(:registers).write 'sp', sp

        value
      end
    end
  end
end
