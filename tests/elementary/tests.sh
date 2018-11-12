#/bin/bash

cd "$(dirname "$0")"

echo
echo "Comparing the demo with your own executable, named projet19, on Error test files"
echo 

nb_e=4
nb_wrong=0

for i in `seq 1 $nb_e`
do
	if [ $i -lt 10 ]
	then
		./../../demo < E0$i.txt > ret1.txt
		./../../projet19 < E0$i.txt > ret2.txt
	else
		./../../demo < E$i.txt > ret1.txt
		./../../projet19 < E$i.txt > ret2.txt
	fi
	cat ret1.txt ret2.txt
	if cmp ret1.txt ret2.txt >/dev/null 2>&1
	then
		echo -e "OK"
	else
		echo -e "!!! FAIL !!!"
		((nb_wrong++))
	fi
	echo
	rm ret1.txt ret2.txt
done

if [ $nb_wrong -ne 0 ]
then
	echo -e "NUMBER OF ERRORS DETECTED: $nb_wrong" 
else
	echo -e "NO ERROR DETECTED"
fi


