#!/bin/bash

BINARY="./memfit"
RESULTS_DIR="./data"
RESULTS_CSV="${RESULTS_DIR}/benchmark_results$(date +%Y%m%d_%H%M%S).csv"

MAX_ALLOC=({1..1000..1})

make

echo "max_alloc,first,next,best,worst" \
    > "${RESULTS_CSV}"
    
extract_result() {
    local output="$1"
    local result_first=$(echo "$output" | grep "FIRST_FIT" | awk '{print $NF}')
    local result_next=$(echo "$output" | grep "NEXT_FIT" | awk '{print $NF}')
    local result_best=$(echo "$output" | grep "BEST_FIT" | awk '{print $NF}')
    local result_worst=$(echo "$output" | grep "WORST_FIT" | awk '{print $NF}')
    echo "${result_first},${result_next},${result_best},${result_worst}"
}


for M in "${MAX_ALLOC[@]}"; do
    echo -n "$M"
    output=$(${BINARY} $M 2>&1)
    result=$(extract_result "$output")
    echo "${M},${result}" >> "${RESULTS_CSV}"
    echo "   OK"
done
