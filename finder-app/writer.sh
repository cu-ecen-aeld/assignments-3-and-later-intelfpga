#!/bin/sh


if [ $# -ne 2 ]
then
	echo "wrong number of args( $#), expected 2 instead"
	exit 1 

fi

file_name=$(basename $1)
dir_name=$(dirname $1)

#echo  "specified file name: ${file_name}"
#echo  "specified directory name ${dir_name}"

#check if dir exists, if not create a new one
if [ ! -d $dir_name ]
then

	mkdir -p $dir_name
fi
	
echo $2 > $1

exit 0 
	
	
