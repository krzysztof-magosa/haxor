module Haxor
  module Compiler
    class Unit
      attr_reader :section

      def initialize
        @section = :text
        @sections = {
          text: [],
          data: [],
          bss: []
        }
      end

      def add(item)
        @sections[@section] << item
      end

      def section=(section)
        fail unless @sections.key? section
        @section = section
      end

      def tokens
        @sections[@section]
      end

      def self.load(filename)
        Marshal.load File.read(filename)
      end

      def save(filename)
        fd = File.open(filename, 'wb')
        fd.write Marshal.dump(self)
        fd.close
      end
    end
  end
end
