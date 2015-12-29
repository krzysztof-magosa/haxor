module Haxor
  module Token
    class Data < Base
      def initialize(data)
        @data = data
      end

      def size
        @data.size
      end

      def to_s
        @data.to_s
      end
    end
  end
end
