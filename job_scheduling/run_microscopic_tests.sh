#!/bin/bash

exe=../build/bin/GA.exe
iter=$2

memory_absolute() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt memory absolute $iter
	$exe ./tests/tests_size/test_15.txt memory absolute $iter
	$exe ./tests/tests_size/test_20.txt memory absolute $iter
	$exe ./tests/tests_size/test_25.txt memory absolute $iter
	$exe ./tests/tests_size/test_50.txt memory absolute $iter
	$exe ./tests/tests_size/test_100.txt memory absolute $iter
	$exe ./tests/tests_size/test_500.txt memory absolute $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt memory absolute $iter
	$exe ./tests/tests_conn/test_0_1.txt memory absolute $iter
	$exe ./tests/tests_conn/test_0_2.txt memory absolute $iter
	$exe ./tests/tests_conn/test_0_3.txt memory absolute $iter
	$exe ./tests/tests_conn/test_0_4.txt memory absolute $iter
	$exe ./tests/tests_conn/test_0_5.txt memory absolute $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt memory absolute $iter
	$exe ./tests/tests_disp/test_5.txt memory absolute $iter
	$exe ./tests/tests_disp/test_10.txt memory absolute $iter
	$exe ./tests/tests_disp/test_15.txt memory absolute $iter
	$exe ./tests/tests_disp/test_20.txt memory absolute $iter
	$exe ./tests/tests_disp/test_25.txt memory absolute $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt memory absolute $iter
	$exe ./tests/tests_ratio/test_0_05.txt memory absolute $iter
	$exe ./tests/tests_ratio/test_0_1.txt memory absolute $iter
	$exe ./tests/tests_ratio/test_0_2.txt memory absolute $iter
	$exe ./tests/tests_ratio/test_0_3.txt memory absolute $iter
	$exe ./tests/tests_ratio/test_0_4.txt memory absolute $iter
}

memory_relative() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt memory relative $iter
	$exe ./tests/tests_size/test_15.txt memory relative $iter
	$exe ./tests/tests_size/test_20.txt memory relative $iter
	$exe ./tests/tests_size/test_25.txt memory relative $iter
	$exe ./tests/tests_size/test_50.txt memory relative $iter
	$exe ./tests/tests_size/test_100.txt memory relative $iter
	$exe ./tests/tests_size/test_500.txt memory relative $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt memory relative $iter
	$exe ./tests/tests_conn/test_0_1.txt memory relative $iter
	$exe ./tests/tests_conn/test_0_2.txt memory relative $iter
	$exe ./tests/tests_conn/test_0_3.txt memory relative $iter
	$exe ./tests/tests_conn/test_0_4.txt memory relative $iter
	$exe ./tests/tests_conn/test_0_5.txt memory relative $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt memory relative $iter
	$exe ./tests/tests_disp/test_5.txt memory relative $iter
	$exe ./tests/tests_disp/test_10.txt memory relative $iter
	$exe ./tests/tests_disp/test_15.txt memory relative $iter
	$exe ./tests/tests_disp/test_20.txt memory relative $iter
	$exe ./tests/tests_disp/test_25.txt memory relative $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt memory relative $iter
	$exe ./tests/tests_ratio/test_0_05.txt memory relative $iter
	$exe ./tests/tests_ratio/test_0_1.txt memory relative $iter
	$exe ./tests/tests_ratio/test_0_2.txt memory relative $iter
	$exe ./tests/tests_ratio/test_0_3.txt memory relative $iter
	$exe ./tests/tests_ratio/test_0_4.txt memory relative $iter
}

memory_forgetting() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt memory forgetting $iter
	$exe ./tests/tests_size/test_15.txt memory forgetting $iter
	$exe ./tests/tests_size/test_20.txt memory forgetting $iter
	$exe ./tests/tests_size/test_25.txt memory forgetting $iter
	$exe ./tests/tests_size/test_50.txt memory forgetting $iter
	$exe ./tests/tests_size/test_100.txt memory forgetting $iter
	$exe ./tests/tests_size/test_500.txt memory forgetting $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt memory forgetting $iter
	$exe ./tests/tests_conn/test_0_1.txt memory forgetting $iter
	$exe ./tests/tests_conn/test_0_2.txt memory forgetting $iter
	$exe ./tests/tests_conn/test_0_3.txt memory forgetting $iter
	$exe ./tests/tests_conn/test_0_4.txt memory forgetting $iter
	$exe ./tests/tests_conn/test_0_5.txt memory forgetting $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt memory forgetting $iter
	$exe ./tests/tests_disp/test_5.txt memory forgetting $iter
	$exe ./tests/tests_disp/test_10.txt memory forgetting $iter
	$exe ./tests/tests_disp/test_15.txt memory forgetting $iter
	$exe ./tests/tests_disp/test_20.txt memory forgetting $iter
	$exe ./tests/tests_disp/test_25.txt memory forgetting $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_05.txt memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_1.txt memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_2.txt memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_3.txt memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_4.txt memory forgetting $iter
}

standard() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt standard $iter
	$exe ./tests/tests_size/test_15.txt standard $iter
	$exe ./tests/tests_size/test_20.txt standard $iter
	$exe ./tests/tests_size/test_25.txt standard $iter
	$exe ./tests/tests_size/test_50.txt standard $iter
	$exe ./tests/tests_size/test_100.txt standard $iter
	$exe ./tests/tests_size/test_500.txt standard $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt standard $iter
	$exe ./tests/tests_conn/test_0_1.txt standard $iter
	$exe ./tests/tests_conn/test_0_2.txt standard $iter
	$exe ./tests/tests_conn/test_0_3.txt standard $iter
	$exe ./tests/tests_conn/test_0_4.txt standard $iter
	$exe ./tests/tests_conn/test_0_5.txt standard $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt standard $iter
	$exe ./tests/tests_disp/test_5.txt standard $iter
	$exe ./tests/tests_disp/test_10.txt standard $iter
	$exe ./tests/tests_disp/test_15.txt standard $iter
	$exe ./tests/tests_disp/test_20.txt standard $iter
	$exe ./tests/tests_disp/test_25.txt standard $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt standard $iter
	$exe ./tests/tests_ratio/test_0_05.txt standard $iter
	$exe ./tests/tests_ratio/test_0_1.txt standard $iter
	$exe ./tests/tests_ratio/test_0_2.txt standard $iter
	$exe ./tests/tests_ratio/test_0_3.txt standard $iter
	$exe ./tests/tests_ratio/test_0_4.txt standard $iter
}

standard          > standard.txt   2>&1
memory_absolute   > absolute.txt   2>&1
memory_relative   > relative.txt   2>&1
memory_forgetting > forgetting.txt 2>&1
