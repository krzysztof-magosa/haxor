module Haxor
  module Token
    class Pointer < Int64
      attr_accessor :data
      attr_reader :label

      def initialize(label)
        super(-1) # invalid pointer
        @label = label
      end
    end
  end
end
