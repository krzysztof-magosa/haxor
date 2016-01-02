module Haxor
  class Consts
    MAJOR = 0
    MINOR = 3
    PATCH = 0
    GEM_VERSION = "#{MAJOR}.#{MINOR}.#{PATCH}"
    VERSION = (MAJOR << 16) | (MINOR << 8) | PATCH

    ROOT_PATH = __dir__ + '/../..'

    WORD_SIZE = 8 # 64bit
    WORD_UNPACK = 'q<'

    RESERVED_MEM = 2048 # 1024 for CPU, 1024 for IVT
    IVT_ADDR     = 1024

    # OpCode
    OPCODE_CMD_MASK = 0x0000_0000_0000_00ff
    OPCODE_FLG_MASK = 0xffff_ffff_ffff_ff00 # << 8
    OPCODE_FLG_OFFSET = 8
    # 8 bits of flag - 1st operand
    # 8 bits of flag - 2nd operand

    OPERAND_FLAGS = 8 # how many bits per operand
    OPERAND_DEREFERENCE = (1 << 0) # Dereference operand (aka *operand)

    # Flags Registry Flags
    FR_ZERO    = 1 << 0 # a-b == 0
    FR_SIGN    = 1 << 1 # a-b < 0
  end
end
