#!/usr/bin/env python3

from subprocess import run, Popen, DEVNULL
from sys import argv
import json

def isInstalled(prog):
	print(f"Checking {prog}... ", end="")
	retcode = Popen(
		("which", prog),
		stdout=DEVNULL,
		stderr=DEVNULL
	).wait()
	found = retcode == 0
	print("Found" if found else "Not found")
	return found

if not isInstalled("nasm"):
	print("fatal: nasm not found")
	exit(1)

if not isInstalled("gcc"):
	print("fatal: gcc not found")
	exit(2)

print("Checking sources... ", end="")

sourcesFound = Popen(
	("ls", "phicalc.c", "utils.asm", "keybinds.json"),
	stdout=DEVNULL
).wait() == 0

print("Found" if sourcesFound else "Not found")

if not sourcesFound:
	print("fatal: not all source files are found")
	exit(3)

print("Parsing keybinds...")

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

print("Creating keybinds_temp.h...")
open("keybinds_temp.h", "w").write(codeTemplate.format(definitions, helpText))

print("Assembling utils.asm...")
run(("nasm", "utils.asm", "-f", "elf64", "-o", "utils.o"))
print("Compiling phicalc.c, linking with utils.o...")
run(("gcc", "phicalc.c", "utils.o", "-o", "phicalc", "-lm", "-O2"))

if "-t" not in argv:
	print("Cleaning up...")
	run(("rm", "utils.o", "keybinds_temp.h"))

print("Success")
