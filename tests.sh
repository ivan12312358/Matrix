#!/bin/bash

for ((i=1;i<=10;i++))
do
res=$(./matrix < ./Tests/test$i/test)
answ=$(cat ./Tests/test$i/answ)
if [ "$answ" = "$res" ]
then
echo "Test $i: Done"
else
echo "Test $i: Failed"
fi
done