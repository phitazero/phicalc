# phicalc

**phicalc** is a Linux-only TUI calculator focused on low-level integer arithmetic.

## Requirements
To run:
- Linux

To build:
- GCC or Clang
- NASM
- Python 3

## Features
- Integer-only arithmetic and bitwise operations
- Supports bases: every base 2 - 16
- Configurable bitness: 8, 16, 32, 64 bits
- Signed and unsigned modes
- Logical/bitwise operators: AND, OR, XOR, shifts
- Supports viewing bit count and log2
- Full keyboard control, configurable via `keybinds.json`
- Access to CPU flags (SF, ZF, CF, OF) after operations
- Customizable color scheme via `colors.h`

## How to build
Clone the code:
```bash
git clone https://github.com/phitazero/phicalc.git
cd phicalc
chmod +x build.py
```
If you have just GCC or Clang (not both) run:
```bash
./build.py
```
If you have both compilers specify which one to use:
```bash
./build.py gcc
# or
./build.py clang
```
You can add the `-t` flag to keep all temporary files.  
Optionally you can edit `keybinds.json` before building.  
**Note: The build script does not validate `keybinds.json`, so edit it carefully.**

## Usage
Run with the `-h` or `--help` flag to get current keybinds.

### Registers
Registers (or `regs`) - integer variables of different purposes. Have variable size, here referred to as 'bitness'.

Main register (`main reg`) - always shown, stores the results of all operations and acts as the first operand. If no operation is selected, numbers are entered into the `main reg`.

Input register (`input reg`) - shown when an operation is selected. If shown, numbers are entered into the `input reg`.

Extension register (`ext reg`) - shown only when non-zero. The higher part of the product of a multiplication is put into the `ext reg`. When the bitness is increased the `ext reg` gets merged with the `main reg`

### Operations
An operation is selected by pressing the corresponding key. When an operation is selected the `input reg` will appear.

Supported operations:

Addition ( `+` )  
Subtraction ( `-` )  
Multiplication ( `*` ) - mode (signed/unsigned) corresponds to the current signedness. The higher half of the product is put into `ext reg`.  
Floor division ( `/` ) mode (signed/unsigned) corresponds to the current signedness.  
Bitwise AND ( `&` )  
Bitwise OR ( `|` )  
Bitwise XOR ( `^` )  
Left shift ( `<` )  
Right logical shift ( `>` ) - if the signedness is unsigned  
Right arithmetic shift ( `>` ) - if the signedness is signed  

### Flags
Flags SF, ZF, CF, OF are tracked.  
Flags are shown like this: SZCO  
If a flag in shown in grey it's unset, if in light magenta - set. If a the flag letter is replaced with a hyphen it's undefined.  
By default all flags are undefined, they are updated after each operation. 


### Keybinds
Can be edited in `keybinds.json` before compiling.

Default keybinds:

| Key       | Action                                                 |
|-----------|--------------------------------------------------------|
| q         | Set bitness to 8 bits (byte)                           |
| w         | Set bitness to 16 bits (word)                          |
| r         | Set bitness to 32 bits (dword)                         |
| t         | Set bitness to 64 bits (qword)                         |
| Q         | Set bitness to 8 bits (byte), preserve sign            |
| W         | Set bitness to 16 bits (word), preserve sign           |
| R         | Set bitness to 32 bits (dword), preserve sign          |
| T         | Set bitness to 64 bits (qword), preserve sign          |
| i         | Set base to binary                                     |
| o         | Set base to decimal                                    |
| p         | Set base to hexadecimal                                |
| l         | Increment base                                         |
| k         | Decrement base                                         |
| space     | Toggle digit grouping                                  |
| s         | Toggle sign                                            |
| S         | Toggle signedness                                      |
| enter     | Perform entered arithmetical or logical operation      |
| C         | Set all registers to zero, clear the entered operation |
| E         | Set the extension register to zero                     |
| D         | Copy the extension register to the main register       |
| backspace | Erase a digit                                          |
| L         | Toggle bit count and log2 view                         |
| X         | Exit                                                   |

### Colors
The color scheme be edited in `colors.h`. Each color is an ANSI color code.

## Examples
All of the following examples use the default color scheme.  
Default state:
![[DWORD DEC S ----]  0000000000](images/example1.png)

Multiplied two unsigned bytes: 150 and 2:
![[BYTE DEC U --CO]  001:044](images/example2.png)

Preparing to add two signed quinary words:
![[WORD QIN S ----]  0000043 + 0000012](images/example3.png)
