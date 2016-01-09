module Haxor
  module Compiler
    class Core
      # Misc
      OP_NOP     = 0x00
      OP_EXITI   = 0x01
      OP_SYSCALL = 0x02

      # Arithmetic
      OP_ADD     = 0x10
      OP_ADDI    = 0x11
      OP_SUB     = 0x12
      OP_MULT    = 0x13
      OP_DIV     = 0x14
      OP_MOD     = 0x15

      # Data transfer
      OP_LW      = 0x20
      OP_SW      = 0x21
      OP_LUI     = 0x22

      # Logical
      OP_AND     = 0x30
      OP_ANDI    = 0x31
      OP_OR      = 0x32
      OP_ORI     = 0x33
      OP_XOR     = 0x34
      OP_NOR     = 0x35
      OP_SLT     = 0x36
      OP_SLTI    = 0x37

      # Bitwise shift
      OP_SLLI    = 0x40
      OP_SRLI    = 0x41
      OP_SLL     = 0x42
      OP_SRL     = 0x43

      # Branch/Jumps
      OP_BEQ     = 0x50
      OP_BEQL    = 0x51
      OP_BNE     = 0x52
      OP_BNEL    = 0x53
      OP_J       = 0x54
      OP_JR      = 0x55
      OP_JAL     = 0x56

      #
      REG_ZERO    = 0
      REG_ASM     = 1

      REG_ARG_0   = 2
      REG_ARG_1   = 3
      REG_ARG_2   = 4
      REG_ARG_3   = 5
      REG_ARG_4   = 6
      REG_ARG_5   = 7
      REG_ARG_6   = 8
      REG_ARG_7   = 9
      REG_ARG_8   = 10
      REG_ARG_9   = 11

      REG_TEMP_0  = 12
      REG_TEMP_1  = 13
      REG_TEMP_2  = 14
      REG_TEMP_3  = 15
      REG_TEMP_4  = 16
      REG_TEMP_5  = 17
      REG_TEMP_6  = 18
      REG_TEMP_7  = 19
      REG_TEMP_8  = 20
      REG_TEMP_9  = 21

      REG_FRAME   = 60
      REG_STACK   = 61
      REG_RETURN  = 62
      REG_SYSCALL = 63

      REG_ALIASES = {
        '$zero' => REG_ZERO,
        '$at'   => REG_ASM,

        '$a0'   => REG_ARG_0,
        '$a1'   => REG_ARG_1,
        '$a2'   => REG_ARG_2,
        '$a3'   => REG_ARG_3,
        '$a4'   => REG_ARG_4,
        '$a5'   => REG_ARG_5,
        '$a6'   => REG_ARG_6,
        '$a7'   => REG_ARG_7,
        '$a8'   => REG_ARG_8,
        '$a9'   => REG_ARG_9,

        '$t0'   => REG_TEMP_0,
        '$t1'   => REG_TEMP_1,
        '$t2'   => REG_TEMP_2,
        '$t3'   => REG_TEMP_3,
        '$t4'   => REG_TEMP_4,
        '$t5'   => REG_TEMP_5,
        '$t6'   => REG_TEMP_6,
        '$t7'   => REG_TEMP_7,
        '$t8'   => REG_TEMP_8,
        '$t9'   => REG_TEMP_9,

        '$fp'  => REG_FRAME,
        '$sp'  => REG_STACK,
        '$ra'  => REG_RETURN,
        '$sc'  => REG_SYSCALL
      }

      def initialize
        @units = []
        @cmds = []
        @autolabel = 0
        @prev_sections = []

        init_cmds
      end

      def init_cmds
        bind_cmd 'section', [:any],       :cmd_section
        bind_cmd 'dw',      [:wildcard],  :cmd_dw
        bind_cmd 'resw',    [:any],       :cmd_resw
        bind_cmd '#',       [:wildcard],  :cmd_rem
        bind_cmd 'push',    [:reg],       :cmd_push
        bind_cmd 'pushi',   [:imm],       :cmd_pushi
        bind_cmd 'pushm',   [:imm],       :cmd_pushm
        bind_cmd 'pop',     [:reg],       :cmd_pop
        bind_cmd 'popm',    [:imm],       :cmd_popm
        bind_cmd 'move',    [:reg, :reg], :cmd_move
        bind_cmd 'clear',   [:reg],       :cmd_clear
        bind_cmd 'not',     [:reg, :reg], :cmd_not
        bind_cmd 'ret',     [],           :cmd_ret
        bind_cmd 'prol',    [:imm],       :cmd_prol
        bind_cmd 'epil',    [],           :cmd_epil

        bind_cmd 'b',       [:imm],             :cmd_b
        bind_cmd 'bal',     [:imm],             :cmd_bal
        bind_cmd 'bgt',     [:reg, :reg, :imm], :cmd_bgt
        bind_cmd 'blt',     [:reg, :reg, :imm], :cmd_blt
        bind_cmd 'bge',     [:reg, :reg, :imm], :cmd_bge
        bind_cmd 'ble',     [:reg, :reg, :imm], :cmd_ble
        bind_cmd 'blez',    [:reg, :imm],       :cmd_blez
        bind_cmd 'bgtz',    [:reg, :imm],       :cmd_bgtz
        bind_cmd 'beqz',    [:reg, :imm],       :cmd_beqz

        bind_cmd 'nop',     [],                 OP_NOP
        bind_cmd 'add',     [:reg, :reg, :reg], OP_ADD
        bind_cmd 'addi',    [:reg, :reg, :imm], OP_ADDI
        bind_cmd 'sub',     [:reg, :reg, :reg], OP_SUB
        bind_cmd 'mult',    [:reg, :reg, :reg], OP_MULT
        bind_cmd 'div',     [:reg, :reg, :reg], OP_DIV
        bind_cmd 'mod',     [:reg, :reg, :reg], OP_MOD
        bind_cmd 'lw',      [:reg, :reg, :imm], OP_LW
        bind_cmd 'sw',      [:reg, :imm, :reg], OP_SW
        bind_cmd 'lui',     [:reg, :imm],       OP_LUI
        bind_cmd 'and',     [:reg, :reg, :reg], OP_AND
        bind_cmd 'andi',    [:reg, :reg, :imm], OP_ANDI
        bind_cmd 'or',      [:reg, :reg, :reg], OP_OR
        bind_cmd 'ori',     [:reg, :reg, :imm], OP_ORI
        bind_cmd 'xor',     [:reg, :reg, :reg], OP_XOR
        bind_cmd 'nor',     [:reg, :reg, :reg], OP_NOR
        bind_cmd 'slt',     [:reg, :reg, :reg], OP_SLT
        bind_cmd 'slti',    [:reg, :reg, :imm], OP_SLTI
        bind_cmd 'slli',    [:reg, :reg, :imm], OP_SLLI
        bind_cmd 'srli',    [:reg, :reg, :imm], OP_SRLI
        bind_cmd 'sll',     [:reg, :reg, :reg], OP_SLL
        bind_cmd 'srl',     [:reg, :reg, :reg], OP_SRL
        bind_cmd 'beq',     [:reg, :reg, :imm], OP_BEQ,  [:rel_imm, :x8]
        bind_cmd 'beql',    [:reg, :reg, :imm], OP_BEQL, [:rel_imm, :x8]
        bind_cmd 'bne',     [:reg, :reg, :imm], OP_BNE,  [:rel_imm, :x8]
        bind_cmd 'bnel',    [:reg, :reg, :imm], OP_BNEL, [:rel_imm, :x8]
        bind_cmd 'j',       [:imm],             OP_J,    [:x8]
        bind_cmd 'jr',      [:reg],             OP_JR
        bind_cmd 'jal',     [:imm],             OP_JAL,  [:x8]
        bind_cmd 'exiti',   [:imm],             OP_EXITI
        bind_cmd 'syscall', [],                 OP_SYSCALL
      end

      def bind_cmd(cmd, args, opcode, opts = [])
        @cmds << {
          cmd: cmd,
          args: args,
          opcode: opcode,
          opts: opts
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

      def cmd_resw(*args)
        (1..args[0].to_i).each do
          add Token::Int64.new(0)
        end
      end

      def cmd_rem(*)
      end

      def cmd_push(*args)
        process_cmd 'addi', ['$sp', '$sp', '-' + Consts::WORD_SIZE.to_s]
        process_cmd 'sw',   ['$sp', '0', args[0]]
      end

      def cmd_pushi(*args)
        process_cmd 'addi', ['$1', '$0', args[0]]
        process_cmd 'addi', ['$sp', '$sp', '-' + Consts::WORD_SIZE.to_s]
        process_cmd 'sw',   ['$sp', '0', '$1']
      end

      def cmd_pushm(*args)
        process_cmd 'lw', ['$1', '$0', args[0]]
        process_cmd 'addi', ['$sp', '$sp', '-' + Consts::WORD_SIZE.to_s]
        process_cmd 'sw',   ['$sp', '0', '$1']
      end

      def cmd_pop(*args)
        process_cmd 'lw', [args[0], '$sp', '0']
        process_cmd 'addi', ['$sp', '$sp', Consts::WORD_SIZE.to_s]
      end

      def cmd_popm(*args)
        process_cmd 'lw', ['$1', '$sp', '0']
        process_cmd 'sw', ['$0', args[0], '$1']
        process_cmd 'addi', ['$sp', '$sp', Consts::WORD_SIZE.to_s]
      end

      def cmd_move(*args)
        process_cmd 'add', [args[0], args[1], '$0']
      end

      def cmd_clear(*args)
        process_cmd 'add', [args[0], '$0', '$0']
      end

      def cmd_not(*args)
        process_cmd 'nor', [args[0], args[1], '$0']
      end

      def cmd_ret(*_args)
        process_cmd 'jr', ['$' + REG_RETURN.to_s]
      end

      def cmd_prol(imm = '0')
        # push $ra
        process_cmd 'addi', ['$sp', '$sp', '-' + Consts::WORD_SIZE.to_s]
        process_cmd 'sw',   ['$sp', '0', '$ra']

        # push $fp
        process_cmd 'addi', ['$sp', '$sp', '-' + Consts::WORD_SIZE.to_s]
        process_cmd 'sw',   ['$sp', '0', '$fp']

        # $fp = $sp
        process_cmd 'addi', ['$fp', '$sp', '0']

        # $sp -= imm
        process_cmd 'addi', ['$sp', '$sp', '-' + imm]
      end

      def cmd_epil(*_args)
        # $sp = $fp
        process_cmd 'addi', ['$sp', '$fp', '0']

        # pop $fp
        process_cmd 'lw', ['$fp', '$sp', '0']
        process_cmd 'addi', ['$sp', '$sp', Consts::WORD_SIZE.to_s]

        # pop $ra
        process_cmd 'lw', ['$ra', '$sp', '0']
        process_cmd 'addi', ['$sp', '$sp', Consts::WORD_SIZE.to_s]

        process_cmd 'ret', []
      end

      def cmd_b(*args)
        process_cmd 'beq', ['$0', '$0', args[0]]
      end

      def cmd_bal(*args)
        process_cmd 'beql', ['$0', '$0', args[0]]
      end

      # branch is greater than
      def cmd_bgt(*args)
        process_cmd 'slt', ['$1', args[1], args[0]]
        process_cmd 'bne', ['$1', '$0', args[2]]
      end

      # branch if less than
      def cmd_blt(*args)
        process_cmd 'slt', ['$1', args[0], args[1]]
        process_cmd 'bne', ['$1', '$0', args[2]]
      end

      # branch is greater or equal
      def cmd_bge(*args)
        process_cmd 'slt', ['$1', args[0], args[1]]
        process_cmd 'beq', ['$1', '$0', args[2]]
      end

      def cmd_ble(*args)
        process_cmd 'slt', ['$1', args[1], args[0]]
        process_cmd 'beq', ['$1', '$0', args[2]]
      end

      def cmd_blez(*args)
        process_cmd 'slt', ['$1', '$0', args[0]]
        process_cmd 'beq', ['$1', '$0', args[1]]
      end

      def cmd_bgtz(*args)
        process_cmd 'slt', ['$1', '$0', args[0]]
        process_cmd 'bne', ['$1', '$0', args[1]]
      end

      def cmd_beqz(*args)
        process_cmd 'beq', [args[0], '$0', args[1]]
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

          add_cmd item, args
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

          if cmd[-1] == ':'
            add Token::Label.new(cmd[0...-1])
          else
            process_cmd(cmd, args)
          end
        end

        @unit.save(filename + '.u')
      end

      def add(token)
        @unit.add token
      end

#      def resolve_arg(a, type)
#        case type
#        when :reg
#          return a[1..-1].to_i
#        when :imm
#          return a.to_i
#        else
#          fail
#        end
#      end

      def add_cmd(info, args = [])
        registers = []
        immediate = 0
        info[:args].each_with_index do |type, index|
          if type == :reg
            if REG_ALIASES.key? args[index]
              registers << REG_ALIASES[args[index]]
            else
              registers << args[index][1..-1].to_i
            end
          elsif type == :imm
            begin
              immediate = parse_number args[index]
            rescue
              immediate = args[index]
            end
          end
        end

        registers.fill 0, registers.size...3

        token      = Token::Cmd.new
        token.cmd  = info[:opcode]
        token.reg1 = registers[0]
        token.reg2 = registers[1]
        token.reg3 = registers[2]
        token.imm  = immediate
        token.opts = info[:opts]

        add token
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
