#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
RESULT=0
DIFF_RES=""

declare -a extra=(
"-s args_flags.c"
"-b -e -n -s -t args_flags.c"
"-t args_flags.c"
"-n args_flags.c"
"-n args_flags.c s21_cat.c"
"no_file.txt"
"-n -b -n args_flags.c"
"-e -n args_flags.c"
"-e args_flags.c"
"-b args_flags.c"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    valgrind --log-file="valgrind_s21_cat.log" --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./s21_cat $t > test_s21_cat.log
    leak=$(grep ERROR valgrind_s21_cat.log)
    (( COUNTER++ ))
    if [[ $leak == *"0 errors from 0 contexts"* ]]
    then
      (( SUCCESS++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m cat $t"
    else
      (( FAIL++ ))
        echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m cat $t"
    fi
    rm test_s21_cat.log
    rm valgrind_s21_cat.log
}

for i in "${extra[@]}"
do
    var="-"
    testing $i
done

echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
