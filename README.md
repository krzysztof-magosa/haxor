# Haxor VM
[![Build Status](https://travis-ci.org/krzysztof-magosa/haxor.svg?branch=master)](https://travis-ci.org/krzysztof-magosa/haxor)

Haxor consists of compiler _hc_ and virtual machine _hvm_.

## Man, why have you written that?
Writing own implementation of VM gives a lot of knowledge about
computer's architecture. You hit into issues not known during day
by day activity in high level languages you use. So... just to
broaden horizons and for fun ;)

## How to install?
You need to install these software before installing Haxor:
* cmake 2.8.7 or newer
* c++11 compatible compiler (e.g. clang 3.5, Apple clang 6.0, gcc 4.8)
* flex (tested on 2.5)
* bison 3.x (the one bundled with OS X is too old, you can use version from homebrew)

_haxor_ is written in c++, you must compile it before use:
```
git clone https://github.com/krzysztof-magosa/haxor.git
cd haxor
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

To specify non-standard Bison location (for example on OS X):
```
cmake -DBISON_EXECUTABLE=/opt/homebrew/Cellar/bison/3.0.4/bin/bison ..
```

## Usage
Compilation:
```
hc program.hax
```

Run:
```
hvm program.hax.e
```

## License
Haxor is licensed under BSD 3-clause license. You can read it [here](LICENSE.txt).

## Architecture

### General information
* Design: RISC, load/store
* Endianness: Little Endian
* WORD size: 64-bit
* Registers size: 64-bit
* Instruction: fixed size, 64-bit
* Arithmetic: integer only, 64-bit
* Memory model: paging with protection flags (read, write, exec)
* Call convention: parameters are passed by $a0-$a9 registers, return values by $v0, $v1

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

|Number|Alias    |Description                                           |
|------|---------|------------------------------------------------------|
|0     |$zero    |always zero register, writes are ignored              |
|1     |$at      |reserved for assembler (pseudo-instructions etc.)     |
|2-11  |$a0-$a9  |argument for routine                                  |
|12-21 |$t0-$t9  |temporary (local variable)                            |
|22-23 |$v0-$v1  |return value                                          |
|24-33 |$s0-$s9  |saved temporary (preserved across function call)      |
|...   |...      |...                                                   |
|60    |$fp      |frame pointer                                         |
|61    |$sp      |stack pointer                                         |
|62    |$ra      |return address for linked jumps/branches              |
|63    |$sc      |syscall function id                                   |

Please always use aliases and not register numbers.

## Memory map
<img src="media/memory.png" width="75%">

|Segment|Read              |Write             |Execute           |
|-------|------------------|------------------|------------------|c
|ivt    |:white_check_mark:|:white_check_mark:|:no_entry_sign:   |
|code   |:no_entry_sign:   |:no_entry_sign:   |:white_check_mark:|
|data   |:white_check_mark:|:white_check_mark:|:no_entry_sign:   |
|bss    |:white_check_mark:|:white_check_mark:|:no_entry_sign:   |
|heap   |:white_check_mark:|:white_check_mark:|:no_entry_sign:   |
|stack  |:white_check_mark:|:white_check_mark:|:no_entry_sign:   |

\* Heap is not implemented yet.

## Language
Haxor uses primitive asm-like syntax. Each command goes into separate line.
You can add comments in code, but they also need to be separate lines, beginning
from _#_. Program starts from _main_ label. Labels are created by putting name followed by colon.

Most of instructions take 3 registers or 2 registers and immediate value.
If not stated differently result goes to first specified register.

## Instructions
### Native instructions
|Syntax|OpCode|Description|
|------|------|-----------|
|nop                  |0x00|Does nothing.|
|-                    |0x01|-|
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
|jalr reg1            |0x57|$ra = pc, goto reg1|
|int num              |0x58|generates software interrupt|
|reti                 |0x59|comes back from interrupt|

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
|bgez reg1, imm     |goto imm if reg1 >= 0|
|li reg1, num       |load number into register|
|la reg1, label     |load address of label into register|
|prol imm           |function prologue, imm - numbers of bytes to reserve on stack|
|epil               |function epilogue|
|resw imm           |reserves `imm` of words|

## System calls
Using _syscall_ command you can run some system calls provided by Haxor VM.
System call number is passed via _$sc_ register, arguments are passed by registers _$a0_-_$a9_.
Return value is written into _$v0_ register, some syscalls return error code in $_v1_.

### print_string (01h)
Print 0 terminated string located under specific address.

Example:
```
li $sc, 01h
la $a0, label_msg
syscall
```

### print_int (02h)
Print number.

Example:
```
li $sc, 02h
li $a0, 123
syscall
```

### read_string (03h)
Reads line from standard input and writes under specified address.
Second parameter designates buffer size (including terminating 0).

Example:
```
li $sc, 03h
la $a0, destination_label
li $a1, 100
syscall
```

### read_int (04h)
Reads integer from standard input.
Value is returned via _$v0_, error code via _$v1_.

Example:
```
li $sc, 04h
syscall
```

### rand (05h)
Generate random number between min and max.

```
li $sc, 05h
li $a0, 100
li $a1, 200
syscall
```

### sleep (06h)
Sleeps for specified number of milliseconds.

```
li $sc, 06h
li $a0, 1000
syscall
```

### exit (07h)
Stops virtual machine and exits with specified exit code.

```
li $sc, 07h
li $a0, 5
syscall
```

### create_timer (08h)
Schedule interrupt to be run every N ticks of system timer (it runs 1000 ticks per second).
Timer identificator is returned in _$v0_ register.

```
la $t0, isr0     # load address of isr0 label into register $t0
sw $zero, 0, $t0 # load $t0 value into memory[0]

li $sc, 08h
li $a0, 0 # int number
syscall
```

### enable_timer (09h)
Enables previously disabled timer.

```
li $sc, 09h
li $a0, 0 # timer id
syscall
```

### disable_timer (0ah)
Disables calling interrupt by timer. Ticks are still counted so it does not impair the cycle..

```
li $sc, 0ah
li $a0, 0 # timer id
syscall
```

### time (0bh)
*Reserved*

### steady_time (0ch)
Returns steady time which cannot be decreased. It's suitable for measuring intervals.

|$a0|Return as    |
|---|-------------|
|0  |hours        |
|1  |minutes      |
|2  |seconds      |
|3  |milliseconds |
|4  |microseconds |
|5  |nanoseconds  |

```
li $sc, 0ch
li $a0, 3
syscall
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
