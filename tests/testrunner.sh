#!/bin/sh




ASM=./asm
EMU=./wcpu


fail=0



testdir="testcases"

testcase() {

	asmfile=$1/*.asm
	outfile=$1/test.bin
	asmbin=$1/assembly.bin
	$ASM $asmfile "-o" "$outfile" > /dev/null

	if cmp $outfile "${asmbin}" > /dev/null 2>&1; then
		emuout=$1/emu.qa
		emucmp=$1/expect.txt

		$EMU "-s" $outfile "-e" $emuout "-test" > /dev/null

		if cmp $emuout $emucmp > /dev/null 2>&1; then
			echo "passed"
		else
			echo "failed"

		fi
	else
		echo "failed build step"


	fi

	return
}


for i in "${testdir}"/*
do
	echo "test:" "$i"
	testcase $i



done