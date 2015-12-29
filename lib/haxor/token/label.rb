module Haxor
  module Token
    class Label < Base
      attr_reader :label

      def initialize(label)
        @label = label
      end

      def to_bytecode
        ''
      end

      def to_s
        "Label -> #{@label}"
      end

      def size
        0
      end
    end
  end
end
