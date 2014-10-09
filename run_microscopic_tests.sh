#!/bin/bash

exe=$1
iter=$2

memory_absolute() {
echo "====================SIZE===================="
$exe ./tests/micro/tests_size/test_10.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_size/test_15.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_size/test_20.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_size/test_25.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_size/test_50.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_size/test_100.txt microscopic memory absolute $iter
#$exe ./tests/micro/tests_size/test_500.txt microscopic memory absolute $iter
echo "====================CONN===================="
$exe ./tests/micro/tests_conn/test_0_0.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_conn/test_0_1.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_conn/test_0_2.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_conn/test_0_3.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_conn/test_0_4.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_conn/test_0_5.txt microscopic memory absolute $iter
echo "====================DISP===================="
$exe ./tests/micro/tests_disp/test_3.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_disp/test_5.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_disp/test_10.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_disp/test_15.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_disp/test_20.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_disp/test_25.txt microscopic memory absolute $iter
echo "====================RATIO===================="
$exe ./tests/micro/tests_ratio/test_0_02.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_ratio/test_0_05.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_ratio/test_0_1.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_ratio/test_0_2.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_ratio/test_0_3.txt microscopic memory absolute $iter
$exe ./tests/micro/tests_ratio/test_0_4.txt microscopic memory absolute $iter
}

memory_relative() {
echo "====================SIZE===================="
$exe ./tests/micro/tests_size/test_10.txt microscopic memory relative $iter
$exe ./tests/micro/tests_size/test_15.txt microscopic memory relative $iter
$exe ./tests/micro/tests_size/test_20.txt microscopic memory relative $iter
$exe ./tests/micro/tests_size/test_25.txt microscopic memory relative $iter
$exe ./tests/micro/tests_size/test_50.txt microscopic memory relative $iter
$exe ./tests/micro/tests_size/test_100.txt microscopic memory relative $iter
#$exe ./tests/micro/tests_size/test_500.txt microscopic memory relative $iter
echo "====================CONN===================="
$exe ./tests/micro/tests_conn/test_0_0.txt microscopic memory relative $iter
$exe ./tests/micro/tests_conn/test_0_1.txt microscopic memory relative $iter
$exe ./tests/micro/tests_conn/test_0_2.txt microscopic memory relative $iter
$exe ./tests/micro/tests_conn/test_0_3.txt microscopic memory relative $iter
$exe ./tests/micro/tests_conn/test_0_4.txt microscopic memory relative $iter
$exe ./tests/micro/tests_conn/test_0_5.txt microscopic memory relative $iter
echo "====================DISP===================="
$exe ./tests/micro/tests_disp/test_3.txt microscopic memory relative $iter
$exe ./tests/micro/tests_disp/test_5.txt microscopic memory relative $iter
$exe ./tests/micro/tests_disp/test_10.txt microscopic memory relative $iter
$exe ./tests/micro/tests_disp/test_15.txt microscopic memory relative $iter
$exe ./tests/micro/tests_disp/test_20.txt microscopic memory relative $iter
$exe ./tests/micro/tests_disp/test_25.txt microscopic memory relative $iter
echo "====================RATIO===================="
$exe ./tests/micro/tests_ratio/test_0_02.txt microscopic memory relative $iter
$exe ./tests/micro/tests_ratio/test_0_05.txt microscopic memory relative $iter
$exe ./tests/micro/tests_ratio/test_0_1.txt microscopic memory relative $iter
$exe ./tests/micro/tests_ratio/test_0_2.txt microscopic memory relative $iter
$exe ./tests/micro/tests_ratio/test_0_3.txt microscopic memory relative $iter
$exe ./tests/micro/tests_ratio/test_0_4.txt microscopic memory relative $iter
}

memory_forgetting() {
echo "====================SIZE===================="
$exe ./tests/micro/tests_size/test_10.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_size/test_15.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_size/test_20.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_size/test_25.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_size/test_50.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_size/test_100.txt microscopic memory forgetting $iter
#$exe ./tests/micro/tests_size/test_500.txt microscopic memory forgetting $iter
echo "====================CONN===================="
$exe ./tests/micro/tests_conn/test_0_0.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_conn/test_0_1.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_conn/test_0_2.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_conn/test_0_3.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_conn/test_0_4.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_conn/test_0_5.txt microscopic memory forgetting $iter
echo "====================DISP===================="
$exe ./tests/micro/tests_disp/test_3.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_disp/test_5.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_disp/test_10.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_disp/test_15.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_disp/test_20.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_disp/test_25.txt microscopic memory forgetting $iter
echo "====================RATIO===================="
$exe ./tests/micro/tests_ratio/test_0_02.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_ratio/test_0_05.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_ratio/test_0_1.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_ratio/test_0_2.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_ratio/test_0_3.txt microscopic memory forgetting $iter
$exe ./tests/micro/tests_ratio/test_0_4.txt microscopic memory forgetting $iter
}

standard() {
echo "====================SIZE===================="
$exe ./tests/micro/tests_size/test_10.txt microscopic standard $iter
$exe ./tests/micro/tests_size/test_15.txt microscopic standard $iter
$exe ./tests/micro/tests_size/test_20.txt microscopic standard $iter
$exe ./tests/micro/tests_size/test_25.txt microscopic standard $iter
$exe ./tests/micro/tests_size/test_50.txt microscopic standard $iter
$exe ./tests/micro/tests_size/test_100.txt microscopic standard $iter
#$exe ./tests/micro/tests_size/test_500.txt microscopic standard $iter
echo "====================CONN===================="
$exe ./tests/micro/tests_conn/test_0_0.txt microscopic standard $iter
$exe ./tests/micro/tests_conn/test_0_1.txt microscopic standard $iter
$exe ./tests/micro/tests_conn/test_0_2.txt microscopic standard $iter
$exe ./tests/micro/tests_conn/test_0_3.txt microscopic standard $iter
$exe ./tests/micro/tests_conn/test_0_4.txt microscopic standard $iter
$exe ./tests/micro/tests_conn/test_0_5.txt microscopic standard $iter
echo "====================DISP===================="
$exe ./tests/micro/tests_disp/test_3.txt microscopic standard $iter
$exe ./tests/micro/tests_disp/test_5.txt microscopic standard $iter
$exe ./tests/micro/tests_disp/test_10.txt microscopic standard $iter
$exe ./tests/micro/tests_disp/test_15.txt microscopic standard $iter
$exe ./tests/micro/tests_disp/test_20.txt microscopic standard $iter
$exe ./tests/micro/tests_disp/test_25.txt microscopic standard $iter
echo "====================RATIO===================="
$exe ./tests/micro/tests_ratio/test_0_02.txt microscopic standard $iter
$exe ./tests/micro/tests_ratio/test_0_05.txt microscopic standard $iter
$exe ./tests/micro/tests_ratio/test_0_1.txt microscopic standard $iter
$exe ./tests/micro/tests_ratio/test_0_2.txt microscopic standard $iter
$exe ./tests/micro/tests_ratio/test_0_3.txt microscopic standard $iter
$exe ./tests/micro/tests_ratio/test_0_4.txt microscopic standard $iter
}

memory_absolute   > micro_absolute.txt   2>&1
memory_relative   > micro_relative.txt   2>&1
memory_forgetting > micro_forgetting.txt 2>&1
standard 	  > micro_standard.txt   2>&1

