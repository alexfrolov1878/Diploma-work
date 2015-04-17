#!/bin/bash

EXE=./build/bin/GA.exe
ITERS=$1

run_tests() {
	CR_TYPE=$1
	MEM_TYPE=$2
	ARGS="${CR_TYPE} ${MEM_TYPE} ${ITERS}"
	echo "====================SIZE===================="
	${EXE} tests/tests_size/test_10.txt ${ARGS}
	${EXE} tests/tests_size/test_15.txt ${ARGS}
	${EXE} tests/tests_size/test_20.txt ${ARGS}
	${EXE} tests/tests_size/test_25.txt ${ARGS}
	${EXE} tests/tests_size/test_50.txt ${ARGS}
	${EXE} tests/tests_size/test_100.txt ${ARGS}
	echo "==================SIZE_BIG=================="
	${EXE} tests/tests_size_big/test_250.txt ${ARGS}
	${EXE} tests/tests_size_big/test_400.txt ${ARGS}
	${EXE} tests/tests_size_big/test_500.txt ${ARGS}
	${EXE} tests/tests_size_big/test_501.txt ${ARGS}
        ${EXE} tests/tests_size_big/test_600.txt ${ARGS}
	${EXE} tests/tests_size_big/test_750.txt ${ARGS}
	echo "====================CONN===================="
	${EXE} tests/tests_conn/test_0_0.txt ${ARGS}
	${EXE} tests/tests_conn/test_0_1.txt ${ARGS}
	${EXE} tests/tests_conn/test_0_2.txt ${ARGS}
	${EXE} tests/tests_conn/test_0_3.txt ${ARGS}
	${EXE} tests/tests_conn/test_0_4.txt ${ARGS}
	${EXE} tests/tests_conn/test_0_5.txt ${ARGS}
	echo "====================DISP===================="
	${EXE} tests/tests_disp/test_3.txt ${ARGS}
	${EXE} tests/tests_disp/test_5.txt ${ARGS}
	${EXE} tests/tests_disp/test_10.txt ${ARGS}
	${EXE} tests/tests_disp/test_15.txt ${ARGS}
	${EXE} tests/tests_disp/test_20.txt ${ARGS}
	${EXE} tests/tests_disp/test_25.txt ${ARGS}
	echo "====================RATIO===================="
	${EXE} tests/tests_ratio/test_0_02.txt ${ARGS}
	${EXE} tests/tests_ratio/test_0_05.txt ${ARGS}
	${EXE} tests/tests_ratio/test_0_1.txt ${ARGS}
	${EXE} tests/tests_ratio/test_0_2.txt ${ARGS}
	${EXE} tests/tests_ratio/test_0_3.txt ${ARGS}
	${EXE} tests/tests_ratio/test_0_4.txt ${ARGS}
}

#CR_TYPES=( 1 2 3 4 5 )
CR_TYPES=( 2 3 4 5 )
MEM_TYPES=( "standard" "memory absolute" "memory relative" "memory forgetting" )

for ((i = 0; i < ${#CR_TYPES[@]}; i++))
do
	CR_TYPE="${CR_TYPES[$i]}"
	for ((j = 0; j < ${#MEM_TYPES[@]}; j++))
	do
		MEM_TYPE="${MEM_TYPES[$j]}"
		FILENAME=`echo "${MEM_TYPE}_${CR_TYPE}" | sed -e "s/ /_/g"`
		run_tests "${CR_TYPE}" "${MEM_TYPE}" > ${FILENAME}.txt 2>&1
	done
done
