module Haxor
  module Vm
    class Os < Subsystem
      TABLE = {
        0x01 => :sys_printf,
        0x02 => :sys_scanf,
        0x03 => :sys_rand
      }

      def syscall
        func = subsystem(:cpu).reg(Vm::Cpu::REG_SYSCALL)
        send(TABLE[func])
      end

      def collect_string(addr)
        i = 0
        string = ''
        loop do
          char = @vm.subsystem(:mem).read(addr + i)
          break if char == 0
          string << char.ord

          i += Consts::WORD_SIZE
        end

        string
      end

      def parse_format(fmt)
        fmt = fmt.tr '^A-Za-z%', ''
        fmt = fmt.gsub '%%', ''

        types = []
        last_char = nil
        fmt.each_char do |char|
          if last_char == '%'
            if %w(b B d i o u x X).include? char
              types << :integer
            elsif %w(c s).include? char
              types << :string
            else
              fail
            end
          end

          last_char = char
        end

        types
      end

      def sys_printf
        fd = subsystem(:stack).pop_value
        x = subsystem(:stack).pop_value
        fmt = collect_string x
        args = []
        parse_format(fmt).each do |type|
          case type
          when :string
            args << collect_string(subsystem(:stack).pop_value)
          when :integer
            args << subsystem(:stack).pop_value
          else
            fail
          end
        end

        file = IO.new(fd, 'a')
        file.write sprintf(fmt, *args)
      end

      def sys_scanf
        fd = subsystem(:stack).pop_value
        fmt = collect_string subsystem(:stack).pop_value
        types = parse_format fmt

        file = IO.new(fd, 'r')
        result = file.scanf fmt

        if result.size != types.size
          subsystem(:cpu).reg Vm::Cpu::REG_SYSCALL, -1
          return
        end

        types.each do |type|
          value = result.shift

          case type
          when :string
            subsystem(:mem).write_string subsystem(:stack).pop_value, value
          when :integer
            subsystem(:mem).write subsystem(:stack).pop_value, value
          else
            fail
          end
        end
      end

      def sys_rand
        min = subsystem(:stack).pop_value
        max = subsystem(:stack).pop_value

        prng = Random.new
        v = prng.rand(min..max)
        subsystem(:stack).push_value v
      end
    end
  end
end
