module Haxor
  module Token
    class Cmd < Base
      attr_accessor :cmd
      attr_accessor :flags
      attr_accessor :reg1
      attr_accessor :reg2
      attr_accessor :reg3
      attr_accessor :imm
      attr_accessor :opts

      def initialize
        @cmd   = 0
        @flags = 0
        @reg1  = 0
        @reg2  = 0
        @reg3  = 0
        @imm   = 0
        @opts  = []
      end

      def size
        Consts::WORD_SIZE
      end

      def to_bytecode
        data = Utils.encode_opcode @cmd, @flags, @reg1, @reg2, @reg3, @imm
        [data].pack Consts::WORD_UNPACK
      end

      def to_s
        "CMD code:[0x#{@cmd.to_s(16)}], flags:[#{@flags.to_s(2)}], regs:[#{@reg1}, #{@reg2}, #{@reg3}], imm:[#{@imm}], opts:#{@opts}"
      end
    end
  end
end
