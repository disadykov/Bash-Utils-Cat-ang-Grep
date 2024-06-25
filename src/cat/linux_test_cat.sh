#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR test_case_cat.txt"
)

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
    ./s21_cat $t > test_s21_cat.log
    cat $t > test_sys_cat.log
    DIFF_RES="$(diff -s test_s21_cat.log test_sys_cat.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_cat.log and test_sys_cat.log are identical" ]
    then
      (( SUCCESS++ ))
        echo -en "\033[0;31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m cat $t\n"
    else
      (( FAIL++ ))
        echo -en "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m cat $t\n"
    fi
    rm test_s21_cat.log test_sys_cat.log
}

# специфические тесты
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

# 1 параметр
for var1 in b e n s t v
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# 2 параметра
for var1 in b e n s t v
do
    for var2 in b e n s t v
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done


echo -en "\033[31mFAIL: $FAIL\033[0m\n"
echo -en "\033[32mSUCCESS: $SUCCESS\033[0m\n"
echo "ALL: $COUNTER"
