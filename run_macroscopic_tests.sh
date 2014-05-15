#!/bin/bash

exe=$1
iter=$2

memory() {
echo "====================SIZE===================="
$exe ./tests/macro/tests_size/test_100.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_200.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_300.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_400.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_500.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_750.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_1000.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_1500.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_2000.txt macroscopic memory $iter
$exe ./tests/macro/tests_size/test_2500.txt macroscopic memory $iter
echo "====================GOAL===================="
$exe ./tests/macro/tests_goal/test_0_1.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_2.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_3.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_4.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_45.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_55.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_6.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_7.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_8.txt macroscopic memory $iter
$exe ./tests/macro/tests_goal/test_0_9.txt macroscopic memory $iter
}

standard() {
echo "====================SIZE===================="
$exe ./tests/macro/tests_size/test_100.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_200.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_300.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_400.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_500.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_750.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_1000.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_1500.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_2000.txt macroscopic standard $iter
$exe ./tests/macro/tests_size/test_2500.txt macroscopic standard $iter
echo "====================GOAL===================="
$exe ./tests/macro/tests_goal/test_0_1.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_2.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_3.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_4.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_45.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_55.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_6.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_7.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_8.txt macroscopic standard $iter
$exe ./tests/macro/tests_goal/test_0_9.txt macroscopic standard $iter
}

standard > macro_standard.txt 2>&1
memory   > macro_memory.txt   2>&1
