# BASH C++ Code Compile Script
# Not tested, but should work.

printf "Compilation started\n"
g++ source.cpp -o compiled -lboost_filesystem -lboost_date_time -lboost_system
printf "Compilation finished\n"