#!/bin/bash

exe=$1
iter=$2

memory() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_100.txt memory $iter
	$exe ./tests/tests_size/test_200.txt memory $iter
	$exe ./tests/tests_size/test_300.txt memory $iter
	$exe ./tests/tests_size/test_400.txt memory $iter
	$exe ./tests/tests_size/test_500.txt memory $iter
	$exe ./tests/tests_size/test_750.txt memory $iter
	$exe ./tests/tests_size/test_1000.txt memory $iter
	$exe ./tests/tests_size/test_1500.txt memory $iter
	$exe ./tests/tests_size/test_2000.txt memory $iter
	$exe ./tests/tests_size/test_2500.txt memory $iter
	echo "====================GOAL===================="
	$exe ./tests/tests_goal/test_0_1.txt memory $iter
	$exe ./tests/tests_goal/test_0_2.txt memory $iter
	$exe ./tests/tests_goal/test_0_3.txt memory $iter
	$exe ./tests/tests_goal/test_0_4.txt memory $iter
	$exe ./tests/tests_goal/test_0_45.txt memory $iter
	$exe ./tests/tests_goal/test_0_55.txt memory $iter
	$exe ./tests/tests_goal/test_0_6.txt memory $iter
	$exe ./tests/tests_goal/test_0_7.txt memory $iter
	$exe ./tests/tests_goal/test_0_8.txt memory $iter
	$exe ./tests/tests_goal/test_0_9.txt memory $iter
}

standard() {
	echo "====================SIZE===================="
	$exe ./tests/tests_size/test_100.txt standard $iter
	$exe ./tests/tests_size/test_200.txt standard $iter
	$exe ./tests/tests_size/test_300.txt standard $iter
	$exe ./tests/tests_size/test_400.txt standard $iter
	$exe ./tests/tests_size/test_500.txt standard $iter
	$exe ./tests/tests_size/test_750.txt standard $iter
	$exe ./tests/tests_size/test_1000.txt standard $iter
	$exe ./tests/tests_size/test_1500.txt standard $iter
	$exe ./tests/tests_size/test_2000.txt standard $iter
	$exe ./tests/tests_size/test_2500.txt standard $iter
	echo "====================GOAL===================="
	$exe ./tests/tests_goal/test_0_1.txt standard $iter
	$exe ./tests/tests_goal/test_0_2.txt standard $iter
	$exe ./tests/tests_goal/test_0_3.txt standard $iter
	$exe ./tests/tests_goal/test_0_4.txt standard $iter
	$exe ./tests/tests_goal/test_0_45.txt standard $iter
	$exe ./tests/tests_goal/test_0_55.txt standard $iter
	$exe ./tests/tests_goal/test_0_6.txt standard $iter
	$exe ./tests/tests_goal/test_0_7.txt standard $iter
	$exe ./tests/tests_goal/test_0_8.txt standard $iter
	$exe ./tests/tests_goal/test_0_9.txt standard $iter
}

standard > standard.txt 2>&1
memory   > memory.txt   2>&1