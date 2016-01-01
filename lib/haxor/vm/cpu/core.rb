module Haxor
  module Vm
    module Cpu
      class Core < Subsystem
        attr_reader :struct
        attr_reader :labels

        def initialize
          @units = []
          @opcodes = {}

          register_unit Unit::Arithmetic.new
          register_unit Unit::Jumps.new
          register_unit Unit::Logical.new
          register_unit Unit::Transfer.new
          register_unit Unit::Various.new

          build_struct
        end

        def bind_opcode(opcode, object, method)
          fail "OpCode already defined - #{opcode}." if @opcodes.key? opcode
          @opcodes[opcode] = {
            object: object,
            method: method
          }
        end

        def iterate
          opcode = subsystem(:mem).next_cell
          cmd = opcode & Consts::OPCODE_CMD_MASK
          subsystem(:mem).write 'op', opcode

          fail "Call to not existing opcode #{cmd}." unless @opcodes.key? cmd
          #        puts 'cmd = ' + cmd.to_s(16)
          #        puts flg.to_s(16)
          @opcodes[cmd][:object].send(@opcodes[cmd][:method])
        end

        def reserved_mem
          1024
        end

        def register_unit(unit)
          @units << unit
          unit.vm = self
          unit.register
        end

        def operand
          operands(1)[0]
        end

        def operands(n = 2)
          flg = subsystem(:mem).read('op') & Consts::OPCODE_FLG_MASK
          flg >> Consts::OPCODE_FLG_OFFSET

          result = []
          n.times do
            v = subsystem(:mem).next_cell
            v = subsystem(:mem).read a if (flg & Consts::OPERAND_DEREFERENCE != 0)
            result << v
            flg << Consts::OPERAND_FLAGS
          end

          result
        end

        private

        def build_struct
          @struct = []

          @struct << Token::Label.new('null')
          @struct << Token::Int64.new(0)

          @struct << Token::Label.new('registers')

          # instruction pointer
          @struct << Token::Label.new('ip')
          @struct << Token::Int64.new(0)

          # stack pointer
          @struct << Token::Label.new('sp')
          @struct << Token::Int64.new(0)

          # base pointer
          @struct << Token::Label.new('bp')
          @struct << Token::Int64.new(0)

          # arithmetic registry I
          @struct << Token::Label.new('ar')
          @struct << Token::Int64.new(0)

          # arithmetic registry II
          @struct << Token::Label.new('dr')
          @struct << Token::Int64.new(0)

          # flags registry
          @struct << Token::Label.new('fr')
          @struct << Token::Int64.new(0)

          # currently processed opcode (with opcode flags)
          @struct << Token::Label.new('op')
          @struct << Token::Int64.new(0)

          # system call
          @struct << Token::Label.new('sc')
          @struct << Token::Int64.new(0)

          # general usage registers
          (1..10).each do |i|
            @struct << Token::Label.new(format('r%02d', i))
            @struct << Token::Int64.new(0)
          end

          addr = 0
          @labels = {}
          @struct.each do |token|
            token.absolute_addr = addr
            addr += token.size

            @labels[token.label] = token if token.is_a? Token::Label
          end
        end
      end
    end
  end
end
