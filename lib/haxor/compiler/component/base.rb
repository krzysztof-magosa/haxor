module Haxor
  module Compiler
    module Component
      class Base
        attr_accessor :compiler

        def register
          fail 'this method must be implemented.'
        end

        def bind_cmd(cmd, method)
          compiler.bind_cmd cmd, self, method
        end

        def add(token)
          compiler.add token
        end

        def parse_value(value)
          compiler.parse_value value
        end

        def offset_flags(a, b = nil)
          compiler.offset_flags(a, b)
        end

        def offset?(value)
          compiler.offset? value
        end

        def strip_offset(value)
          compiler.strip_offset value
        end
      end
    end
  end
end
