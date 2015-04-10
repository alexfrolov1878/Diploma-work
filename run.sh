set -x

#cd job_scheduling
#./run_tests.sh 250
#mv *.txt results/91/
#./run_tests.sh 250
#mv *.txt results/92/
#./run_tests.sh 250
#mv *.txt results/93/
#./run_tests.sh 250
#mv *.txt results/94/
#cd ..

cd subset_sum
./run_tests.sh 100
mv *.txt results/91/
./run_tests.sh 100
mv *.txt results/92/
./run_tests.sh 100
mv *.txt results/93/
./run_tests.sh 100
mv *.txt results/94/
cd ..

set +x
