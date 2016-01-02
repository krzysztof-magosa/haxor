module Haxor
  module Compiler
    class Core
      def initialize
        @units = []
        @cmds = []
        @autolabel = 0
        @prev_sections = []

        init_cmds
      end

      def init_cmds
        bind_cmd 'section', [:any],       :cmd_section
        bind_cmd 'label',   [:any],       :cmd_label
        bind_cmd 'dw',      [:wildcard],  :cmd_dw
        bind_cmd 'resw',    [:any],       :cmd_resw
        bind_cmd '#',       [:wildcard],  :cmd_rem
        bind_cmd 'rem',     [:wildcard],  :cmd_rem
        bind_cmd 'mov',     [:any, :any], Vm::Cpu::Unit::Transfer::OP_MOV
        bind_cmd 'push',    [:any],       Vm::Cpu::Unit::Transfer::OP_PUSH
        bind_cmd 'pop',     [:any],       Vm::Cpu::Unit::Transfer::OP_POP
        bind_cmd 'and',     [:any, :any], Vm::Cpu::Unit::Logical::OP_AND
        bind_cmd 'neg',     [:any],       Vm::Cpu::Unit::Logical::OP_NEG
        bind_cmd 'not',     [:any],       Vm::Cpu::Unit::Logical::OP_NOT
        bind_cmd 'or',      [:any, :any], Vm::Cpu::Unit::Logical::OP_OR
        bind_cmd 'xor',     [:any, :any], Vm::Cpu::Unit::Logical::OP_XOR
        bind_cmd 'shl',     [:any, :any], Vm::Cpu::Unit::Logical::OP_SHL
        bind_cmd 'shr',     [:any, :any], Vm::Cpu::Unit::Logical::OP_SHR
        bind_cmd 'nop',     [],           Vm::Cpu::Unit::Various::OP_NOP
        bind_cmd 'lea',     [:any, :any], Vm::Cpu::Unit::Various::OP_LEA
        bind_cmd 'int',     [:any],       Vm::Cpu::Unit::Various::OP_INT
        bind_cmd 'syscall', [],           Vm::Cpu::Unit::Various::OP_SYSCALL
        bind_cmd 'call',    [:any],       Vm::Cpu::Unit::Jumps::OP_CALL
        bind_cmd 'ret',     [],           Vm::Cpu::Unit::Jumps::OP_RET
        bind_cmd 'iret',    [],           Vm::Cpu::Unit::Jumps::OP_IRET
        bind_cmd 'jmp',     [:any],       Vm::Cpu::Unit::Jumps::OP_JMP
        bind_cmd 'je',      [:any],       Vm::Cpu::Unit::Jumps::OP_JE
        bind_cmd 'jg',      [:any],       Vm::Cpu::Unit::Jumps::OP_JG
        bind_cmd 'jge',     [:any],       Vm::Cpu::Unit::Jumps::OP_JGE
        bind_cmd 'jl',      [:any],       Vm::Cpu::Unit::Jumps::OP_JL
        bind_cmd 'jle',     [:any],       Vm::Cpu::Unit::Jumps::OP_JLE
        bind_cmd 'jne',     [:any],       Vm::Cpu::Unit::Jumps::OP_JNE
        bind_cmd 'jng',     [:any],       Vm::Cpu::Unit::Jumps::OP_JNG
        bind_cmd 'jnge',    [:any],       Vm::Cpu::Unit::Jumps::OP_JNGE
        bind_cmd 'jnl',     [:any],       Vm::Cpu::Unit::Jumps::OP_JNL
        bind_cmd 'jnle',    [:any],       Vm::Cpu::Unit::Jumps::OP_JNLE
        bind_cmd 'inc',     [:any],       Vm::Cpu::Unit::Arithmetic::OP_INC
        bind_cmd 'dec',     [:any],       Vm::Cpu::Unit::Arithmetic::OP_DEC
        bind_cmd 'add',     [:any, :any], Vm::Cpu::Unit::Arithmetic::OP_ADD
        bind_cmd 'sub',     [:any, :any], Vm::Cpu::Unit::Arithmetic::OP_SUB
        bind_cmd 'div',     [:any],       Vm::Cpu::Unit::Arithmetic::OP_DIV
        bind_cmd 'mul',     [:any],       Vm::Cpu::Unit::Arithmetic::OP_MUL
        bind_cmd 'cmp',     [:any, :any], Vm::Cpu::Unit::Arithmetic::OP_CMP
      end

      def bind_cmd(cmd, args, opcode)
        @cmds << {
          cmd: cmd,
          args: args,
          opcode: opcode
        }
      end

      def cmd_section(name)
        @unit.section = name[1..-1].to_sym
      end

      def cmd_dw(*args)
        add Token::Label.new(args[0])

        if args.size == 1
          add Token::Int64.new(0)
        else
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
      end

      def cmd_label(name)
        add Token::Label.new(name)
      end

      def cmd_resw(*args)
        (1..args[0].to_i).each do
          add Token::Int64.new(0)
        end
      end

      def cmd_rem(*)
      end

      # this is ugly and must be reworked
      def split_arguments(string)
        inside_quotes = false
        splits = []
        string.each_char.with_index(0) do |x, i|
          if x == '"' && string[i - 1] != '\\'
            inside_quotes ^= true
          end

          if x == ',' && !inside_quotes
            splits << i
          end
        end

        splits << string.size

        last_x = 0
        args = []
        splits.each do |x|
          args << string[last_x...x]
          last_x = x + 1
        end
        args.map!(&:strip).delete_if { |x| x.length == 0 }
      end

      def process_cmd(cmd, args)
        @cmds.each do |item|
          next if item[:cmd] != cmd

          if item[:opcode].is_a? Symbol
            send(item[:opcode], *args)
            return
          end

          add_cmd item[:opcode], args
          return
        end

        puts cmd
        fail
      end

      def compile(filename)
        input = File.read(filename, encoding: 'ASCII-8BIT')

        @unit = Unit.new filename

        input.lines.map(&:chomp).each_with_index do |line, index|
          next if line.empty?

          tmp = line.split ' ', 2
          cmd = tmp[0]
          args = split_arguments(tmp[1] || '')

          process_cmd(cmd, args)
        end

        @unit.save(filename + '.u')
      end

      def add(token)
        @unit.add token
      end

      def add_cmd(opcode, args = [])
        opcode |= offset_flags(args[0], args[1])
        add Token::Cmd.new(opcode)
        parse_value args[0] unless args[0].nil?
        parse_value args[1] unless args[1].nil?
      end

      def parse_number(value)
        md = value.match(/\A([-+]?[0-9]+[0-9A-F]*)([bhd]?)\z/)
        fail if md.nil?

        case md[2]
        when 'b'
          base = 2
        when 'h'
          base = 16
        when 'd'
        else
          base = 10
        end

        md[1].to_i(base)
      end

      def parse_value(value)
        value = strip_offset value

        begin
          number = parse_number(value)
          @autolabel += 1
          label = "__autolabel_#{@unit.hash}_#{@autolabel}"

          old_section = @unit.section
          @unit.section = :data
          add Token::Label.new(label)
          add Token::Int64.new(number)
          @unit.section = old_section

          add Token::Pointer.new(label)
        rescue
          add Token::Pointer.new(value)
        end
      end

      def offset_flags(*operands)
        result = 0
        flag = Consts::OPERAND_DEREFERENCE << Consts::OPCODE_FLG_OFFSET

        operands.each do |operand|
          result |= flag if offset?(operand)
          flag << Consts::OPERAND_FLAGS
        end

        result
      end

      def offset?(value)
        return false unless value.is_a? String
        value.start_with?('[') && value.end_with?(']')
      end

      def strip_offset(value)
        return value[1...-1] if offset?(value)
        value
      end
    end
  end
end
