module Haxor
  module Vm
    module Cpu
      class Core < Subsystem
        attr_accessor :ip

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

        REG_ZERO    = 0
        REG_STACK   = 61
        REG_RETURN  = 62
        REG_SYSCALL = 63

        def initialize
          @registers = Array.new(64, 0)
          @units = []
          @opcodes = {}
          init_opcodes
        end

        def init_opcodes
          bind_opcode OP_NOP, :op_nop
          bind_opcode OP_ADD, :op_add
          bind_opcode OP_ADDI, :op_addi
          bind_opcode OP_SUB, :op_sub
          bind_opcode OP_MULT, :op_mult
          bind_opcode OP_DIV, :op_div
          bind_opcode OP_MOD, :op_mod
          bind_opcode OP_LW, :op_lw
          bind_opcode OP_SW, :op_sw
          bind_opcode OP_LUI, :op_lui
          bind_opcode OP_AND, :op_and
          bind_opcode OP_ANDI, :op_andi
          bind_opcode OP_OR, :op_or
          bind_opcode OP_ORI, :op_ori
          bind_opcode OP_XOR, :op_xor
          bind_opcode OP_NOR, :op_nor
          bind_opcode OP_SLT, :op_slt
          bind_opcode OP_SLTI, :op_slti
          bind_opcode OP_SLLI, :op_slli
          bind_opcode OP_SRLI, :op_srli
          bind_opcode OP_SLL, :op_sll
          bind_opcode OP_SRL, :op_srl
          bind_opcode OP_BEQ, :op_beq
          bind_opcode OP_BEQL, :op_beql
          bind_opcode OP_BNE, :op_bne
          bind_opcode OP_BNEL, :op_bnel
          bind_opcode OP_J, :op_j
          bind_opcode OP_JR, :op_jr
          bind_opcode OP_JAL, :op_jal
          bind_opcode OP_EXITI, :op_exiti
          bind_opcode OP_SYSCALL, :op_syscall
        end

        def op_nop(info)
        end

        def op_add(info)
          reg info.reg1, reg(info.reg2) + reg(info.reg3)
        end

        def op_addi(info)
          reg info.reg1, reg(info.reg2) + info.imm
        end

        def op_sub(info)
          reg info.reg1, reg(info.reg2) - reg(info.reg3)
        end

        def op_mult(info)
          reg info.reg1, reg(info.reg2) * reg(info.reg3)
        end

        def op_div(info)
          reg info.reg1, reg(info.reg2) / reg(info.reg3)
        end

        def op_mod(info)
          reg info.reg1, reg(info.reg2) % reg(info.reg3)
        end

        # reg1 = memory[reg2+imm]
        def op_lw(info)
          addr = reg(info.reg2) + info.imm
          reg info.reg1, subsystem(:mem).read(addr)
        end

        # memory[reg1+imm] = reg2
        def op_sw(info)
          addr = reg(info.reg1) + info.imm
          subsystem(:mem).write addr, reg(info.reg2)
        end

        def op_lui(info)
          reg info.reg1, (info.imm << 32)
        end

        def op_and(info)
          reg info.reg1, reg(info.reg2) & reg(info.reg3)
        end

        def op_andi(info)
          reg info.reg1, reg(info.reg2) & info.imm
        end

        def op_or(info)
          reg info.reg1, reg(info.reg2) | reg(info.reg3)
        end

        def op_ori(info)
          reg info.reg1, reg(info.reg2) | info.imm
        end

        def op_xor(info)
          reg info.reg1, reg(info.reg2) ^ reg(info.reg3)
        end

        def op_nor(info)
          reg info.reg1, ~(reg(info.reg2) | reg(info.reg3))
        end

        def op_slt(info)
          reg info.reg1, reg(info.reg2) < reg(info.reg3) ? 1 : 0
        end

        def op_slti(info)
          reg info.reg1, reg(info.reg2) < info.imm ? 1 : 0
        end

        def op_slli(info)
          reg info.reg1, reg(info.reg2) << info.imm
        end

        def op_srli(info)
          reg info.reg1, reg(info.reg2) >> info.imm
        end

        def op_sll(info)
          reg info.reg1, reg(info.reg2) << reg(info.reg3)
        end

        def op_srl(info)
          reg info.reg1, reg(info.reg2) >> reg(info.reg3)
        end

        def op_beq(info)
          branch(info.imm) if reg(info.reg1) == reg(info.reg2)
        end

        def op_beql(info)
          return if reg(info.reg1) != reg(info.reg2)

          reg REG_RETURN, @ip
          branch(info.imm)
        end

        def op_bne(info)
          branch info.imm if reg(info.reg1) != reg(info.reg2)
        end

        def op_bnel(info)
          return if reg(info.reg1) == reg(info.reg2)

          reg REG_RETURN, @ip
          branch(info.imm)
        end

        def op_j(info)
          jmp info.imm
        end

        def op_jr(info)
          @ip = reg(info.reg1)
        end

        def op_jal(info)
          reg REG_RETURN, @ip
          jmp info.imm
        end

        def op_exiti(info)
          exit info.imm
        end

        def op_syscall(_info)
          @vm.subsystem(:os).syscall
        end

        def reg(id, value = nil)
          return @registers[id] if value.nil?
          return if id == REG_ZERO # $0 cannot be overwritten
          @registers[id] = value
        end

        def jmp(addr)
          @ip = addr * Consts::WORD_SIZE
        end

        def branch(rel_addr)
          @ip += rel_addr * Consts::WORD_SIZE
        end

        def bind_opcode(opcode, method)
          fail "OpCode already defined - #{opcode}." if @opcodes.key? opcode
          @opcodes[opcode] = {
            method: method
          }
        end

        def iterate
          opcode = subsystem(:mem).read @ip
          @ip += Consts::WORD_SIZE
          info = Utils.decode_opcode opcode
          send @opcodes[info.cmd][:method], info
        end
      end
    end
  end
end
