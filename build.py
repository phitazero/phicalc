#!/usr/bin/env python3

from subprocess import run, Popen, DEVNULL
from sys import argv
import json

retcode = Popen(
	("which", "nasm"),
	stdout=DEVNULL,
	stderr=DEVNULL
).wait()
if retcode != 0:
	print("nasm not found")
	exit(1)

retcode = Popen(
	("ls", "phicalc.c", "utils.asm", "keybinds.json"),
	stdout=DEVNULL
).wait()
if retcode != 0:
	print("not all source files are found")
	exit(2)

keybinds = json.load(open("keybinds.json", "r"))

definitions = ""
helpText = ""

for macro, keybind in keybinds.items():
	keyValue = keybind["key"]
	cLiteral = f"\'{keyValue}\'" if type(keyValue) == str else str(keyValue)
	definitions += f"#define {macro} {cLiteral}\n"
	key = keybind["key"] if "alias" not in keybind else keybind["alias"]
	helpText += f"{key} - {keybind["desc"]}\\n"

codeTemplate = """{0}

void printKeybinds() {{
	printf("{1}");
}}
"""

open("keybinds_temp.h", "w").write(codeTemplate.format(definitions, helpText))

run(("nasm", "utils.asm", "-f", "elf64", "-o", "utils.o"))
run(("gcc", "phicalc.c", "utils.o", "-o", "phicalc", "-lm", "-O2"))

if "-t" not in argv:
	run(("rm", "utils.o", "keybinds_temp.h"))
