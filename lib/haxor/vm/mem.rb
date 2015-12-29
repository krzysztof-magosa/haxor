module Haxor
  module Vm
    class Mem < Subsystem
      attr_accessor :memory

      def initialize(bytes)
        @memory = Array.new(bytes, 0).pack('C*')
        @labels = {}
      end

      def size
        @memory.size
      end

      def enlarge(bytes)
        replace_region size, Array.new(bytes, 0).pack('C*')
      end

      # Adds label
      #
      # @param name [String] Name of label
      # @param addr [Integer] Address in memory to point to
      def add_label(name, addr)
        fail if @labels.key? name
        @labels[name] = addr
      end

      # Reads word from memory
      #
      # @param addr [String|Integer] Label or address in memory
      # @return [Integer] Data stored under specified address
      def read(addr)
        @memory[resolve addr].unpack(Consts::WORD_UNPACK)[0]
      end

      # Writes word to memory
      #
      # @param addr [String|Integer] Label or address in memory
      # @param value [Integer] Data to store under specified address
      def write(addr, value)
        @memory[resolve addr] = [value].pack(Consts::WORD_UNPACK)
      end

      def write_string(addr, value)
        i = 0
        value.each_char do |x|
          write addr + i, x.ord
          i += Consts::WORD_SIZE
        end

        write addr + i, 0
      end

      def replace_region(addr, data)
        x = addr + data.size
        @memory[addr...x] = data
      end

      # (see #read)
      def dereference(addr)
        read addr
      end

      # Reads data pointed by IP register and moves IP forward
      #
      # @return [Integer] Data stored under address pointed by IP
      def next_word
        data = read ip
        move_ip Consts::WORD_SIZE
        data
      end

      # TODO: BC
      def next_cell
        next_word
      end

      def ip
        read 'ip'
      end

      def move_ip(step)
        write 'ip', ip + step
      end

      def label(name)
        @labels[addr]
      end

      private

      def resolve(addr)
        if addr.is_a? String
          fail "Unknown label #{addr}." unless @labels.key? addr
          addr = @labels[addr]
        end
        Range.new(addr, addr + Consts::WORD_SIZE, true)
      end
    end
  end
end
