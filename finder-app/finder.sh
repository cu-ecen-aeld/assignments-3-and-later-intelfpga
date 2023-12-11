#!/bin/sh

if [ $# -ne 2 ]
then 
	echo "Required number of args is 2 not $# "
	exit 1
else
	if [ -d $1 ]
	then
		echo "directory path exsists" 
	
	else
	
		echo "First argument is not a directory"
		exit 1
	fi
fi 

num_of_files=$(ls $1 | wc -l )
num_of_lines=$(grep -rnw $1 -e $2 | wc -l)

#echo $num_of_files
#echo $num_of_lines

echo "The number of files are ${num_of_files} and the number of matching lines are ${num_of_lines}"
exit 0
