set -x

cd job_scheduling
./run_tests.sh 250
mv *.txt results/1/
./run_tests.sh 250
mv *.txt results/2/
./run_tests.sh 250
mv *.txt results/3/
./run_tests.sh 250
mv *.txt results/4/
cd ..

cd subset_sum
./run_tests.sh 100
mv *.txt results/1/
./run_tests.sh 100
mv *.txt results/2/
./run_tests.sh 100
mv *.txt results/3/
./run_tests.sh 100
mv *.txt results/4/
cd ..

set +x
