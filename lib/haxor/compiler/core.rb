module Haxor
  module Compiler
    class Core
      def initialize
        @units = []
        @cmds = {}
        @autolabel = 0
        @prev_sections = []

        bind_cmd 'section', self, :cmd_section
      end

      def register_unit(unit)
        @units << unit
        unit.compiler = self
        unit.register
      end

      def bind_cmd(cmd, object, method)
        @cmds[cmd] = {
          object: object,
          method: method
        }
      end

      def cmd_section(name)
        @unit.section = name[1..-1].to_sym
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

      def compile(filename)
        input = File.read(filename, encoding: 'ASCII-8BIT')

        @unit = Unit.new

        input.lines.map(&:chomp).each_with_index do |line, index|
          next if line.empty?

          tmp = line.split ' ', 2
          cmd = tmp[0]
          args = split_arguments(tmp[1] || '')

          fail "Unknown command #{cmd} in line #{index}." unless @cmds.key? cmd

          @cmds[cmd][:object].send(@cmds[cmd][:method], *args)
        end

        @unit.save(filename + '.u')
      end

      def add(token)
        @unit.add token
      end

      def add_cmd(opcode, a = nil, b = nil)
        opcode |= offset_flags(a, b)
        add Token::Cmd.new(opcode)
        parse_value a unless a.nil?
        parse_value b unless b.nil?
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
          label = "__autolabel_#{@autolabel}"

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

      def offset_flags(a, b = nil)
        result = 0
        result |= Consts::OPCODE_FLG_DA if offset?(a)
        result |= Consts::OPCODE_FLG_DB if offset?(b)
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
