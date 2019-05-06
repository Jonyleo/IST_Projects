# Creator: Jonyleo 

# This script compiles and runs your C program using the required flags for IAED class (Based on teacher's scripts)
# usage: ./runProgram.sh <file.c> 			{without tests}
# usage: ./runProgram.sh <file.c> <test.in> 

# also make sure to have no files names that start with tmp_runProgram  in the folder or they will be DELETED

prog_name="${1}"
test="${2}"
compFile="tmp_runProgram${RANDOM}"
compFlags="gcc -ansi -Wall -Wextra -pedantic"

if ! [ -f ${cfile} ] ; then
	echo "./run.sh: ${cfile} does not exist"
	exit 1

else 
	if [ -f "tmp_runProgram"* ] ; then
		rm "tmp_runProgram"*
	fi
	${compFlags} -o ${compFile} ${prog_name}
	if [ -f ${compFile} ] ; then
		echo "./runProgram.sh: Program successfully compiled..."
		echo "./runProgram.sh: Running..."
		if [ $# -eq 2 -a -f ${test} ] ; then
			echo "./runProgram.sh: Test ${test}"
			echo ""
			./${compFile}  <"${test}"
		else
			echo "./runProgram.sh: No tests..."
			echo ""
			./${compFile}
		fi
		rm ${compFile}
	else
		echo "./runProgram.sh: ERROR: Compilation failed!"
	fi
fi