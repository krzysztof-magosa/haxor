# Haxor VM
[![Gem Version](https://badge.fury.io/rb/haxor.svg)](https://badge.fury.io/rb/haxor)
[![Code Climate](https://codeclimate.com/github/krzysztof-magosa/haxor/badges/gpa.svg)](https://codeclimate.com/github/krzysztof-magosa/haxor)

Haxor consists of compiler _hc_, linker _hld_ and virtual machine _hvm_.
_hc_ translates asm-like code into tokens, _hld_ links them into bytecode, while _hvm_ runs it.

## Man, why have you written that?
Writing own implementation of VM gives a lot of knowledge about
computer's architecture. You hit into issues not known during day
by day activity in high level languages you use. So... just to
broaden horizons and for fun ;)

## How to install?
You need to install these software before installing Haxor:
* Ruby 2.x
* cmake 2.8.7 or newer
* c++11 compiler (I use clang++ bundled with OS X)

_hc_ and _hld_ are written in ruby, you can install them by gem:
```
gem install haxor
```

_hvm_ is written in c++, you must compile it before use:
```
cd vm
mkdir build
cd build
cmake ..
make
make install
```

Optionally you can change installation prefix by invoking this command instead of `cmake ..`:
```
cmake -DCMAKE_INSTALL_PREFIX=/opt/haxor ..
```

## Usage
Compilation:
```
hcc program.hax
```

Linking:
```
hld -o program.hax.e program.hax.u
hld -s 4096 -o program.hax.e program.hax.u # custom stack size
```

Run:
```
hvm program.hax.e
```

## License
Haxor is licensed under BSD 3-clause license. You can read it [here](LICENSE.txt).

## Architecture

### General information
* Design: RISC
* Endianness: Little Endian
* WORD size: 64-bit
* Registers size: 64-bit
* Instruction: fixed size, 64-bit
* Arithmetic: integer only, 64-bit
* Memory model: flat, no protection
* Syscall call convention: similar to [cdecl](https://en.wikipedia.org/wiki/X86_calling_conventions#cdecl)

### OpCodes
Instruction is 64-bit, and contains:
* 0:6 bits - instruction code (7 bits, unsigned)
* 7:8 bits - flags (2 bits, unsigned, not used at the moment)
* 9:14 bits - register 1 (6 bits, unsigned)
* 15:20 bits - register 2 (6 bits, unsigned)
* 21:26 bits - register 3 (6 bits, unsigned)
* 27:63 bits - immediate value (37 bits, signed)

### vCPU
vCPU has 64 registers, some of them have special role:

|Number|Alias|Description|
|------|-----|-----------|
|0     |$zero|always zero register, writes are ignored|
|1     |$at  |reserved for assembler (pseudo-instructions etc.)|
|2     |$a0  |argument for routine|
|3     |$a1  |argument for routine|
|4     |$a2  |argument for routine|
|5     |$a3  |argument for routine|
|6     |$a4  |argument for routine|
|7     |$a5  |argument for routine|
|8     |$a6  |argument for routine|
|9     |$a7  |argument for routine|
|10    |$a8  |argument for routine|
|11    |$a9  |argument for routine|
|12    |$t0  |temporary (local variable)|
|13    |$t1  |temporary (local variable)|
|14    |$t2  |temporary (local variable)|
|15    |$t3  |temporary (local variable)|
|16    |$t4  |temporary (local variable)|
|17    |$t5  |temporary (local variable)|
|18    |$t6  |temporary (local variable)|
|19    |$t7  |temporary (local variable)|
|20    |$t8  |temporary (local variable)|
|21    |$t9  |temporary (local variable)|
|...   |...  |...|
|61    |$sp  |stack pointer|
|62    |$ra  |return address for linked jumps/branches|
|63    |$sc  |syscall function id and return code|

## Memory map
<img src="media/memory.png" width="50%">

## Language
Haxor uses primitive asm-like syntax. Each command goes into separate line.
You can add comments in code, but they also need to be separate lines, beginning
from _#_. Program starts from _main_ label. Labels are created by putting name and color on the end of line (e.g. `main:`).

Most of instructions take 3 registers or 2 registers and immediate value.
If not stated differently result goes to first specified register.

## Instructions
### Native instructions
|Syntax|OpCode|Description|
|------|------|-----------|
|nop                  |0x00|Does nothing.|
|exiti imm            |0x01|Closes VM with specified exit code.|
|syscall              |0x02|Performs Syscall with ID stored in $sc register.|
|add reg1, reg2, reg3 |0x10|reg1 = reg2 + reg3|
|addi reg1, reg2, imm |0x11|reg1 = reg2 + imm|
|sub reg1, reg2, reg3 |0x12|reg1 = reg2 - reg3|
|mult reg1, reg2, reg3|0x13|reg1 = reg2 * reg3|
|div reg1, reg2, reg3 |0x14|reg1 = reg2 / reg3|
|mod reg1, reg2, reg3 |0x15|reg1 = reg2 % reg3|
|lw reg1, reg2, imm   |0x20|reg1 = memory[reg2 + imm]|
|sw reg1, imm, reg2   |0x21|memory[reg1+imm] = reg2|
|lui reg1, imm        |0x22|reg1 = (imm << 32)|
|and reg1, reg2, reg3 |0x30|reg1 = reg2 & reg3|
|andi reg1, reg2, imm |0x31|reg1 = reg2 & imm|
|or reg1, reg2, reg3  |0x32|reg1 = reg2 \| reg3|
|ori reg1, reg2, imm  |0x33|reg1 = reg2 \| imm|
|xor reg1, reg2, reg3 |0x34|reg1 = reg2 ^ reg3|
|nor reg1, reg2, reg3 |0x35|reg1 = ~(reg2 \| reg3)|
|slt reg1, reg2, reg3 |0x36|reg1 = reg2 < reg3|
|slti reg1, reg2, imm |0x37|reg1 = reg2 < imm|
|slli reg1, reg2, imm |0x40|reg1 = reg2 << imm|
|srli reg1, reg2, imm |0x41|reg1 = reg2 >> imm|
|sll reg1, reg2, reg3 |0x42|reg1 = reg2 << reg3|
|srl reg1, reg2, reg3 |0x43|reg1 = reg2 >> reg3|
|beq reg1, reg2, imm  |0x50|goto imm if reg1 == reg2|
|beql reg1, reg2, imm |0x51|$ra = pc, goto imm if reg1 == reg2|
|bne reg1, reg2, imm  |0x52|goto imm if reg1 != reg2|
|bnel reg1, reg2, imm |0x53|$ra = pc, goto imm if reg1 != reg2|
|j imm                |0x54|goto imm|
|jr reg1              |0x55|goto reg1|
|jal imm              |0x56|$ra = pc, goto imm|

### Pseudo instructions
|Syntax|Description|
|------|-----------|
|push reg1          |Pushes register onto stack|
|pushi imm          |Pushes const onto stack|
|pushm imm          |Pushes word stored at specified address|
|pop reg1           |Pops value into register|
|popm imm           |Pops value into specified address|
|move reg1, reg2    |reg1 = reg2|
|clear reg1         |reg1 = 0|
|not reg1, reg2     |reg1 = ~reg2|
|ret                |Jumps to address stored in $ra|
|b imm              |Unconditional branch|
|bal imm            |Unconditional linked branch|
|bgt reg1, reg2, imm|goto imm if reg1 > reg2|
|blt reg1, reg2, imm|goto imm if reg1 < reg2|
|bge reg1, reg2, imm|goto imm if reg1 >= reg2|
|ble reg1, reg2, imm|goto imm if reg1 <= reg2|
|blez reg1, imm     |goto imm if reg1 <= 0|
|bgtz reg1, imm     |goto imm if reg1 > 0|
|beqz reg1, imm     |goto imm if reg1 == 0|
|prol imm           |function prologue, imm - numbers of bytes to reserve on stack|
|epil               |function epilogue|

## System calls
Using _syscall_ command you can run some system calls provided by Haxor VM.
System call number is passed via _$sc_ register, arguments go via stack in reversed order.
Return value is written into _$sc_ register.

### printf (01h)
Prints formatted text into file specified by descriptor.
Takes 2 or more arguments:
* file descriptor (1 for standard output, 2 for standard error)
* format string
* data depending on format string...

Example:
```
addi $sc, $0, 01h
pushi msg_fmt
pushi 1
syscall
addi $sc, $sc, 16
```

### scanf (02h)
Converts data from file specified by descriptor.
Remember that memory is not automatically
allocated by this function. You need to prepare
space before calling this function.
Use length limits to avoid buffer overflow (e.g. %100s to take up to 100 characters from string).
In case of string your buffer must have 1 element more for closing '0'.
Takes 2 or more arguments:
* file descriptor (0 for standard input)
* format string
* addresses in memory to put data into them...

Example:
```
addi $sc, $0, 02h
pushi answer
pushi format
pushi 0
syscall
addi $sc, $sc, 24
```

### random (03h)
Generates random integer from specified range.
Arguments:
* minimum (inclusive)
* maximum (inclusive)

Generated number is written to _$sc_ register.

Example:
```
addi $sc, $0, 03h
pushi 100
pushi 1
syscall
addi $sc, $sc, 16
```

## Useful knowledge related to (virtual) machines
* [Reduced instruction set computing](https://en.wikipedia.org/wiki/Reduced_instruction_set_computing)
* [Sigil](https://en.wikipedia.org/wiki/Sigil_(computer_programming))
* [Endianness](https://en.wikipedia.org/wiki/Endianness)
* [Berkeley RISC](https://en.wikipedia.org/wiki/Berkeley_RISC)
* [Comparison of instruction set architectures](https://en.wikipedia.org/wiki/Comparison_of_instruction_set_architectures)
* [Flag field](https://en.wikipedia.org/wiki/Flag_field)
* [Delay slot](https://en.wikipedia.org/wiki/Delay_slot)
* [Processor register](https://en.wikipedia.org/wiki/Processor_register)
* [Index register](https://en.wikipedia.org/wiki/Index_register)
* [Instruction set](https://en.wikipedia.org/wiki/Instruction_set)
* [ARM architecture](https://en.wikipedia.org/wiki/ARM_architecture)
* [Arithmetic logic unit](https://en.wikipedia.org/wiki/Arithmetic_logic_unit)
* [Processor design](https://en.wikipedia.org/wiki/Processor_design)
* [Minimal instruction set computer](https://en.wikipedia.org/wiki/Minimal_instruction_set_computer)
* [Opcode](https://en.wikipedia.org/wiki/Opcode)
* [Computer architecture](https://en.wikipedia.org/wiki/Computer_architecture)
* [NX bit](https://en.wikipedia.org/wiki/NX_bit)
* [Register memory architecture](https://en.wikipedia.org/wiki/Register_memory_architecture)
* [Vector processor](https://en.wikipedia.org/wiki/Vector_processor)
* [Instruction set](https://en.wikipedia.org/wiki/Instruction_set)
* [VDSO](https://en.wikipedia.org/wiki/VDSO)
* [Bytecode](https://en.wikipedia.org/wiki/Bytecode)
* [Virtual machine](https://en.wikipedia.org/wiki/Virtual_machine)
* [Stack machine](https://en.wikipedia.org/wiki/Stack_machine)
* [Zero instruction set computer](https://en.wikipedia.org/wiki/Zero_instruction_set_computer)
* [System call](https://en.wikipedia.org/wiki/System_call)
* [P-code machine](https://en.wikipedia.org/wiki/P-code_machine)
* [Accumulator](https://en.wikipedia.org/wiki/Accumulator_(computing))
* [Register machine](https://en.wikipedia.org/wiki/Register_machine)
* [Memory segmentation](https://en.wikipedia.org/wiki/Memory_segmentation)
* [Zero page](https://en.wikipedia.org/wiki/Zero_page)
* [Status register](https://en.wikipedia.org/wiki/Status_register)
* [Call stack](https://en.wikipedia.org/wiki/Call_stack)
* [Virtual memory](https://en.wikipedia.org/wiki/Virtual_memory)
* [Stride_of_an_array](https://en.wikipedia.org/wiki/Stride_of_an_array)
* [Relocation](https://en.wikipedia.org/wiki/Relocation_(computing))
* [Zero_address_arithmetic](https://en.wikipedia.org/wiki/Zero_address_arithmetic)
* [Addressing_mode](https://en.wikipedia.org/wiki/Addressing_mode)
* [Nibble](https://en.wikipedia.org/wiki/Nibble)
* [.bss](https://en.wikipedia.org/wiki/.bss)
