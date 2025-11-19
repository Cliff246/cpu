#!/bin/bash




ASM=./asm
EMU=./wcpu


fail=0



testdir="testcases"

function testcase {

	local asmfile="${1}/*.asm"
	local outfile="${1}/test.bin"
	local asmbin="${1}/assembly.bin"
	$ASM $asmfile "-o" $outfile

	cmp $outfile ${asmbin}

	local emuout="${1}/emu.qa"
	local emucmp="${1}/expect.txt"
	$EMU "-s" $outfile "-e" $emuout "-test"
	echo "completed"
	return
}


for i in "${testdir}"/*
do
	echo $i
	testcase $i



done