module Haxor
  module Compiler
    module Component
      class Other < Base
        def register
          bind_cmd 'label', :cmd_label
          bind_cmd 'rem', :cmd_rem
          bind_cmd '#', :cmd_rem
        end

        def cmd_rem(*_args)
          # nothing
        end

        def cmd_label(*args)
          add Token::Label.new(args[0])
        end
      end
    end
  end
end
