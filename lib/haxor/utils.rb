module Haxor
  module Utils
    def self.encode_opcode(cmd = 0, flags = 0, reg1 = 0, reg2 = 0, reg3 = 0, immediate = 0)
      result = cmd
      result |= flags << 8
      result |= reg1 << 14
      result |= reg2 << 20
      result |= reg3 << 26
      result |= immediate << 32
      result
    end

    def self.decode_opcode(opcode)
      [
        opcode & 0xff,         # cmd
        (opcode >> 8) & 0x3f,  # flags
        (opcode >> 14) & 0x3f, # reg1
        (opcode >> 20) & 0x3f, # reg2
        (opcode >> 26) & 0x3f, # reg3
        (opcode >> 32)         # immediate
      ]
    end
  end
end
