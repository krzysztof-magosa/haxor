module Haxor
  class Header
    MAGIC = 0x72_65_88_79_82 # HAXOR (ASCII)
    SIZE = 64 # max 8 values (64bit per entry)

    attr_accessor :version
    attr_accessor :entry_point
    attr_accessor :bss_size
    attr_accessor :stack_size

    def initialize
      @magic = MAGIC
    end

    def dump
      result = ''
      result << [
        @magic,
        @version,
        @entry_point,
        @bss_size,
        @stack_size
      ].pack('q<*')
      result.ljust(SIZE)
    end

    def parse!(data)
      data = data.unpack('q<*')
      @magic       = data.shift
      @version     = data.shift
      @entry_point = data.shift
      @bss_size    = data.shift
      @stack_size  = data.shift
    end

    def size
      SIZE
    end
  end
end
