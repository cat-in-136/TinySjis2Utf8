# Uncomment lines below if you have problems with $PATH
#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)

all: compile_commands.json sjis2utf8.tbl
	pio -f -c vim run

upload:
	pio -f -c vim run --target upload

clean:
	rm compile_commands.json
	pio -f -c vim run --target clean

program:
	pio -f -c vim run --target program

uploadfs:
	pio -f -c vim run --target uploadfs

update:
	pio -f -c vim update

test: sjis2utf8.tbl
	pio -f -c vim test

compile_commands.json:
	pio -f -c vim run --target compiledb

sjis2utf8.tbl: create_sjis2utf8tbl.py
	python3 create_sjis2utf8tbl.py

.PHONY: all upload clean program uploadfs update test
