module Haxor
  module Token
    class Int64 < Data
      def to_bytecode
        [@data].pack('q<')
      end

      def size
        8
      end

      def to_s
        "int64 -> #{@data}"
      end
    end
  end
end
