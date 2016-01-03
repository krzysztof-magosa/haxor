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

    IVT_MEM      = 1024
    IVT_ADDR     = 0
    RESERVED_MEM = IVT_MEM
  end
end
