#!/bin/sh
if [ $1 -le 0 ] || [ $2 -le 0 ]
then
  echo "Inputs are not in the right range!"
else
  for i in $(seq 1 $1)
  do
    for j in $(seq 1 $2)
    do
    mult=`expr $i \* $j`
    printf "$i*$j=$mult "
    done
  printf "\n"
  done
fi

exit 0

