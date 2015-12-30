module Haxor
  module Vm
    class Core
      attr_reader :mem

      def initialize
        @subsystems = {}
        register_subsystem :cpu, Cpu::Core.new
        register_subsystem :mem, Mem.new(Consts::RESERVED_MEM)
        register_subsystem :stack, Stack.new
        register_subsystem :os, Os.new
        register_subsystem :registers, Registers.new

        subsystem(:cpu).labels.each do |_, label|
          subsystem(:mem).add_label label.label, label.absolute_addr
          subsystem(:registers).add_register label.label, label.absolute_addr
        end

        @units = []
        @opcodes = {}
      end

      def register_subsystem(id, object)
        @subsystems[id] = object
        object.vm = self
      end

      def subsystem(id)
        @subsystems[id]
      end

      def execute
        loop do
          subsystem(:cpu).iterate
        end
      end

      def load_program(filename)
        exe = File.read(filename, encoding: 'ASCII-8BIT')

        @hdr = Header.new
        @hdr.parse! exe

        fail if @hdr.version != Consts::VERSION

        exe = exe[@hdr.size..-1] # cut off header
        subsystem(:mem).replace_region Consts::RESERVED_MEM, exe
        subsystem(:registers).write 'ip', @hdr.entry_point # instruction pointer
        subsystem(:mem).enlarge @hdr.bss_size
        subsystem(:mem).enlarge @hdr.stack_size
        subsystem(:registers).write 'sp', subsystem(:mem).size
      end
    end
  end
end
