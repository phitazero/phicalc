# phicalc

**phicalc** is a Linux-only TUI calculator focused on low-level integer arithmetic.

## Requirements
- Linux
- GCC
- NASM
- Python 3 (to build, optional for masochists)

## Features
- Integer-only arithmetic and bitwise operations
- Supports bases: every base 2 - 16
- Configurable bitness: 8, 16, 32, 64 bits
- Signed and unsigned modes
- Logical/bitwise operators: AND, OR, XOR, shifts
- Supports viewing bit count and log2
- Full keyboard control, configurable via `keybinds.json`

## How to build
Clone the repo, cd into its directory and enter this:
```bash
chmod +x build.py
./build.py
```
or this:
```bash
python3 build.py
```
You can add the -t flag to keep all temporary files. 
Optionally you can edit `keybinds.json` before building.
Note: The build script does not validate `keybinds.json`, so edit it carefully.

## Usage
### Registers
Registers (or `regs`) - integer variables of different purposes. Have variable size, here referred to as 'bitness'.

Main register (`main reg`) - always shown, stores the results of all operations and acts as the first operand. If no operation is selected, numbers are entered into the `main reg`.

Input register (`input reg`) - shown when an operation is selected. If shown, numbers are entered into the `input reg`.

Extension register (`ext reg`) - shown only when non-zero. The higher part of the product of a multiplication is put into the `ext reg`.

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

### Keybinds
Can be edited in `keybinds.json` before compiling.

Default keybinds:
```
q - Set bitness to 8 bits (byte)
w - Set bitness to 16 bits (word)
r - Set bitness to 32 bits (dword)
t - Set bitness to 64 bits (qword)
Q - Set bitness to 8 bits (byte), preserve sign
W - Set bitness to 16 bits (word), preserve sign
R - Set bitness to 32 bits (dword), preserve sign
T - Set bitness to 64 bits (qword), preserve sign
i - Set base to binary
o - Set base to decimal
p - Set base to hexadecimal
l - Increment base
k - Decrement base
space - Toggle digit grouping
s - Toggle sign
S - Toggle signedness
enter - Perform entered arithmetical or logical operation
C - Set all registers to zero, clear the entered operation
E - Set the extension register to zero
D - Copy the extension register to the main register
backspace - Erase a digit
L - Toggle bit count and log2 view
X - Exit
```
