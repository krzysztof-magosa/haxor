module Haxor
  module Vm
    class Registers < Subsystem
      def initialize
        @registers = {}
      end

      def flag(flag, enabled)
        flags = read 'fr'
        if enabled
          flags |= flag
        else
          flags &= ~flag
        end
        write 'fr', flags
      end

      def check_flag(flag)
        flags = read 'fr'
        (flags & flag) > 0
      end

      def read(register)
        addr = resolve register
        @vm.subsystem(:mem).read addr
      end

      def write(register, value)
        addr = resolve register
        @vm.subsystem(:mem).write addr, value
      end

      def resolve(register)
        @registers[register]
      end

      def add_register(register, addr)
        @registers[register] = addr
      end
    end
  end
end
