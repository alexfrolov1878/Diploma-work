#!/bin/bash

exe=./build/bin/GA.exe
iter=$1
cr_type=1

memory_absolute() {
	echo "====================SIZE===================="
	echo "$exe ./tests/tests_size/test_10.txt $cr_type memory absolute $iter"
	$exe ./tests/tests_size/test_10.txt $cr_type memory absolute $iter
	$exe ./tests/tests_size/test_15.txt $cr_type memory absolute $iter
	$exe ./tests/tests_size/test_20.txt $cr_type memory absolute $iter
	$exe ./tests/tests_size/test_25.txt $cr_type memory absolute $iter
	$exe ./tests/tests_size/test_50.txt $cr_type memory absolute $iter
	$exe ./tests/tests_size/test_100.txt $cr_type memory absolute $iter
	#$exe ./tests/tests_size/test_500.txt $cr_type $cr_type memory absolute $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt $cr_type memory absolute $iter
	$exe ./tests/tests_conn/test_0_1.txt $cr_type memory absolute $iter
	$exe ./tests/tests_conn/test_0_2.txt $cr_type memory absolute $iter
	$exe ./tests/tests_conn/test_0_3.txt $cr_type memory absolute $iter
	$exe ./tests/tests_conn/test_0_4.txt $cr_type memory absolute $iter
	$exe ./tests/tests_conn/test_0_5.txt $cr_type memory absolute $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt $cr_type memory absolute $iter
	$exe ./tests/tests_disp/test_5.txt $cr_type memory absolute $iter
	$exe ./tests/tests_disp/test_10.txt $cr_type memory absolute $iter
	$exe ./tests/tests_disp/test_15.txt $cr_type memory absolute $iter
	$exe ./tests/tests_disp/test_20.txt $cr_type memory absolute $iter
	$exe ./tests/tests_disp/test_25.txt $cr_type memory absolute $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt $cr_type memory absolute $iter
	$exe ./tests/tests_ratio/test_0_05.txt $cr_type memory absolute $iter
	$exe ./tests/tests_ratio/test_0_1.txt $cr_type memory absolute $iter
	$exe ./tests/tests_ratio/test_0_2.txt $cr_type memory absolute $iter
	$exe ./tests/tests_ratio/test_0_3.txt $cr_type memory absolute $iter
	$exe ./tests/tests_ratio/test_0_4.txt $cr_type memory absolute $iter
}

memory_relative() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt $cr_type memory relative $iter
	$exe ./tests/tests_size/test_15.txt $cr_type memory relative $iter
	$exe ./tests/tests_size/test_20.txt $cr_type memory relative $iter
	$exe ./tests/tests_size/test_25.txt $cr_type memory relative $iter
	$exe ./tests/tests_size/test_50.txt $cr_type memory relative $iter
	$exe ./tests/tests_size/test_100.txt $cr_type memory relative $iter
	#$exe ./tests/tests_size/test_500.txt $cr_type memory relative $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt $cr_type memory relative $iter
	$exe ./tests/tests_conn/test_0_1.txt $cr_type memory relative $iter
	$exe ./tests/tests_conn/test_0_2.txt $cr_type memory relative $iter
	$exe ./tests/tests_conn/test_0_3.txt $cr_type memory relative $iter
	$exe ./tests/tests_conn/test_0_4.txt $cr_type memory relative $iter
	$exe ./tests/tests_conn/test_0_5.txt $cr_type memory relative $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt $cr_type memory relative $iter
	$exe ./tests/tests_disp/test_5.txt $cr_type memory relative $iter
	$exe ./tests/tests_disp/test_10.txt $cr_type memory relative $iter
	$exe ./tests/tests_disp/test_15.txt $cr_type memory relative $iter
	$exe ./tests/tests_disp/test_20.txt $cr_type memory relative $iter
	$exe ./tests/tests_disp/test_25.txt $cr_type memory relative $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt $cr_type memory relative $iter
	$exe ./tests/tests_ratio/test_0_05.txt $cr_type memory relative $iter
	$exe ./tests/tests_ratio/test_0_1.txt $cr_type memory relative $iter
	$exe ./tests/tests_ratio/test_0_2.txt $cr_type memory relative $iter
	$exe ./tests/tests_ratio/test_0_3.txt $cr_type memory relative $iter
	$exe ./tests/tests_ratio/test_0_4.txt $cr_type memory relative $iter
}

memory_forgetting() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_size/test_15.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_size/test_20.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_size/test_25.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_size/test_50.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_size/test_100.txt $cr_type memory forgetting $iter
	#$exe ./tests/tests_size/test_500.txt $cr_type memory forgetting $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_conn/test_0_1.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_conn/test_0_2.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_conn/test_0_3.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_conn/test_0_4.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_conn/test_0_5.txt $cr_type memory forgetting $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_disp/test_5.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_disp/test_10.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_disp/test_15.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_disp/test_20.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_disp/test_25.txt $cr_type memory forgetting $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_05.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_1.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_2.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_3.txt $cr_type memory forgetting $iter
	$exe ./tests/tests_ratio/test_0_4.txt $cr_type memory forgetting $iter
}

standard() {
	echo "$exe ./tests/tests_size/test_10.txt $cr_type memory absolute $iter"
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_10.txt $cr_type standard $iter
	$exe ./tests/tests_size/test_15.txt $cr_type standard $iter
	$exe ./tests/tests_size/test_20.txt $cr_type standard $iter
	$exe ./tests/tests_size/test_25.txt $cr_type standard $iter
	$exe ./tests/tests_size/test_50.txt $cr_type standard $iter
	$exe ./tests/tests_size/test_100.txt $cr_type standard $iter
	#$exe ./tests/tests_size/test_500.txt $cr_type standard $iter
	echo "====================CONN===================="
	$exe ./tests/tests_conn/test_0_0.txt $cr_type standard $iter
	$exe ./tests/tests_conn/test_0_1.txt $cr_type standard $iter
	$exe ./tests/tests_conn/test_0_2.txt $cr_type standard $iter
	$exe ./tests/tests_conn/test_0_3.txt $cr_type standard $iter
	$exe ./tests/tests_conn/test_0_4.txt $cr_type standard $iter
	$exe ./tests/tests_conn/test_0_5.txt $cr_type standard $iter
	echo "====================DISP===================="
	$exe ./tests/tests_disp/test_3.txt $cr_type standard $iter
	$exe ./tests/tests_disp/test_5.txt $cr_type standard $iter
	$exe ./tests/tests_disp/test_10.txt $cr_type standard $iter
	$exe ./tests/tests_disp/test_15.txt $cr_type standard $iter
	$exe ./tests/tests_disp/test_20.txt $cr_type standard $iter
	$exe ./tests/tests_disp/test_25.txt $cr_type standard $iter
	echo "====================RATIO===================="
	$exe ./tests/tests_ratio/test_0_02.txt $cr_type standard $iter
	$exe ./tests/tests_ratio/test_0_05.txt $cr_type standard $iter
	$exe ./tests/tests_ratio/test_0_1.txt $cr_type standard $iter
	$exe ./tests/tests_ratio/test_0_2.txt $cr_type standard $iter
	$exe ./tests/tests_ratio/test_0_3.txt $cr_type standard $iter
	$exe ./tests/tests_ratio/test_0_4.txt $cr_type standard $iter
}

standard          #> standard.txt   2>&1
#memory_absolute   > absolute.txt   2>&1
#memory_relative   > relative.txt   2>&1
#memory_forgetting > forgetting.txt 2>&1