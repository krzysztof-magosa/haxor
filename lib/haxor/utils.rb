module Haxor
  module Utils
    def self.encode_opcode(cmd = 0, flags = 0, reg1 = 0, reg2 = 0, reg3 = 0, imm = 0)
      result = cmd
      result |= flags << 7
      result |= reg1 << 9
      result |= reg2 << 15
      result |= reg3 << 21
      result |= imm << 27
      result
    end

    def self.decode_opcode(opcode)
      r = OpenStruct.new
      r.cmd   = opcode & 0x7f
      r.flags = (opcode >> 7) & 0x03
      r.reg1  = (opcode >> 9) & 0x3f
      r.reg2  = (opcode >> 15) & 0x3f
      r.reg3  = (opcode >> 21) & 0x3f
      r.imm   = (opcode >> 27)
      r
    end
  end
end
