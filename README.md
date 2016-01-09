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
* $0 - always zero register, writes are ignored
* $61 (alias $sp) - stack pointer
* $62 (alias $ra) - return address for linked jumps
* $63 (alias $sc) - syscall function id and return code

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

## System calls
Using _syscall_ command you can run some system calls provided by Haxor VM.
System call number is passed via _$sc_ register, arguments go via stack in reversed order.

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
```

### random (03h)
Generates random integer from specified range.
Arguments:
* minimum (inclusive)
* maximum (inclusive)

Generated number is pushed onto stack.

Example:
```
addi $sc, $0, 03h
pushi 100
pushi 1
syscall
```
