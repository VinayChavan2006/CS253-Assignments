#!/bin/bash

# function to count the length of common prefix of two strings
function count_common_prefix(){
	# store the input string args
	local str1=$1
	local str2=$2
	local count=0

	# get the length of str1 and str2
	local len1=${#str1}
	local len2=${#str2}
	local min_len=$((len1<len2 ? len1:len2))
	
	# compare characters one by one
	for ((i=0;i<min_len;i++)) 
	do
		if [ "${str1:$i:1}" != "${str2:$i:1}" ] # {str1:$i:1} is like slicing from str1[i] of length 1
		then
			break
		fi
		((count++))
	done
	echo "$count"
	return "$count"
}

# Check if exactly two args are provided
if [ $# -ne 2 ] # $# for number of args
then
	echo "Usage: $0 file1.txt file2.txt"
	exit 1
fi

file1=$1
file2=$2
output_file="prefix_counts.txt"

# check if both files exists
if [ ! -f $file1 ] || [ ! -f $file2 ]
then
	echo "Usage: $0 file1.txt file2.txt"
	echo "One or both files do not exist"
	exit 1
fi

# clear the output_file
> $output_file

while IFS= read -r line1 <&3 # read file from file descriptor 3
do
	IFS= read -r line2 <&4 || line2="" # read file from file descriptor 4

	# calculate the common prefix length and append to output file
	count_common_prefix "$line1" "$line2"
	prefix_len=$?

	# write the prefix_len to output_file
	echo $prefix_len >> $output_file
done 3<$file1 4<$file2 # open the file1, file2 and bind it to file descriptor 3,4 respectively


