module Haxor
  module Compiler
    module Component
      class Data < Base
        def register
          bind_cmd 'dw', :cmd_dw
          bind_cmd 'resw', :cmd_resw
        end

        def cmd_dw(*args)
          add Token::Label.new(args[0])

          (1...args.size).each do |i|
            begin
              add Token::Int64.new(Integer(args[i]))
            rescue
              args[i][1...-1].each_char do |c|
                add Token::Int64.new(c.ord)
              end
            end
          end
        end

        def cmd_resw(*args)
          (1..args[0].to_i).each do
            add Token::Int64.new(0)
          end
        end
      end
    end
  end
end
