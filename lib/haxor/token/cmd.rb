module Haxor
  module Token
    class Cmd < Int64
      def to_s
        "Cmd -> 0x#{@data.to_s(16)}"
      end
    end
  end
end
