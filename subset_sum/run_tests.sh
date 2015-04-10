#!/bin/bash

EXE=./build/bin/GA.exe
ITERS=$1

run_tests() {
	CR_TYPE=$1
	MEM_TYPE=$2
	ARGS="${CR_TYPE} ${MEM_TYPE} ${ITERS}"
	#echo "====================SIZE===================="
	#${EXE} tests/tests_size/test_100.txt ${ARGS}
	#${EXE} tests/tests_size/test_200.txt ${ARGS}
	#${EXE} tests/tests_size/test_300.txt ${ARGS}
	#${EXE} tests/tests_size/test_400.txt ${ARGS}
	#${EXE} tests/tests_size/test_500.txt ${ARGS}
	#${EXE} tests/tests_size/test_600.txt ${ARGS}
	#${EXE} tests/tests_size/test_700.txt ${ARGS}
	#${EXE} tests/tests_size/test_800.txt ${ARGS}
	#${EXE} tests/tests_size/test_900.txt ${ARGS}
	#${EXE} tests/tests_size/test_1000.txt ${ARGS}
	#${EXE} tests/tests_size/test_1500.txt ${ARGS}
	#${EXE} tests/tests_size/test_2000.txt ${ARGS}
	#${EXE} tests/tests_size/test_2500.txt ${ARGS}
	echo "==================SIZE_BIG=================="
	${EXE} tests/tests_size_big/test_5000.txt ${ARGS}
	${EXE} tests/tests_size_big/test_6000.txt ${ARGS}
	${EXE} tests/tests_size_big/test_7500.txt ${ARGS}
	${EXE} tests/tests_size_big/test_9000.txt ${ARGS}
	${EXE} tests/tests_size_big/test_10000.txt ${ARGS}
	#echo "====================GOAL===================="
	#${EXE} tests/tests_goal/test_0_0.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_1.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_2.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_3.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_4.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_45.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_55.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_6.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_7.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_8.txt ${ARGS}
	#${EXE} tests/tests_goal/test_0_9.txt ${ARGS}
	#${EXE} tests/tests_goal/test_1_0.txt ${ARGS}
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
