#!/bin/bash

BINARY="./memfit_time"
RESULTS_DIR="./data"
RESULTS_CSV="${RESULTS_DIR}/benchmark_results_2d$(date +%Y%m%d_%H%M%S).csv"

MAX_ALLOC=({10..1000..5})
MEM_SIZE=({10..1000..5})

make

echo "mem_size,max_alloc,first,next,best,worst,first_time,next_time,best_time,worst_time" \
    > "${RESULTS_CSV}"
    
extract_result() {
    local output="$1"
    local result_first=$(echo "$output" | grep "FIRST_FIT_COUNT" | awk '{print $NF}')
    local result_next=$(echo "$output"  | grep "NEXT_FIT_COUNT"  | awk '{print $NF}')
    local result_best=$(echo "$output"  | grep "BEST_FIT_COUNT"  | awk '{print $NF}')
    local result_worst=$(echo "$output" | grep "WORST_FIT_COUNT" | awk '{print $NF}')
    local result_time_first=$(echo "$output" | grep "FIRST_FIT_TIME" | awk '{print $NF}')
    local result_time_next=$(echo "$output"  | grep "NEXT_FIT_TIME"  | awk '{print $NF}')
    local result_time_best=$(echo "$output"  | grep "BEST_FIT_TIME"  | awk '{print $NF}')
    local result_time_worst=$(echo "$output" | grep "WORST_FIT_TIME" | awk '{print $NF}')
    echo "${result_first},${result_next},${result_best},${result_worst},${result_time_first},${result_time_next},${result_time_best},${result_time_worst}"
}


for M in "${MEM_SIZE[@]}"; do
    for A in "${MAX_ALLOC[@]}"; do
        echo -n "$M $A"
        output=$(${BINARY} $A $M 2>&1)
        result=$(extract_result "$output")
        echo "${M},${A},${result}" >> "${RESULTS_CSV}"
        echo "   OK"
    done
done