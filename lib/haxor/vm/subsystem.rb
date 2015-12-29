module Haxor
  module Vm
    class Subsystem
      attr_accessor :vm

      def register
        # method called after registering to VM
      end

      # shortcut
      def subsystem(id)
        @vm.subsystem(id)
      end
    end
  end
end
