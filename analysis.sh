# traces = [tiny, testgen, ls]
# algos=("cache" "sample" "empty")
algos=("cache" "sample" "empty")
trace=$1
# algo="sample"

echo "Compiling all"
make

for algo in "${algos[@]}"; do
	echo "#################(${algo})"
	source run.sh ${algo} ${trace} > /dev/null
	echo "finished running"
	log_file="logs/${algo}_${trace}.txt"
	tail -n 7 $log_file
	echo "--Amount of prefetcher requests: $(grep "hasRequest is planning to " ${log_file} | wc -l)"
	echo "--Amount of misses: $(grep "req.HitL1=0" ${log_file} | wc -l)"
done
echo "#################"

# for i in algos
# 	print last x lines to get stats
# 	grep wc amount of misses
# 	grep wc amount of requests
