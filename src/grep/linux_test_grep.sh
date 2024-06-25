#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a extra=(
"check Makefile func_grep.c"
"-i WhIlE Makefile func_grep.c"
"-v s21 Makefile"
"-c s Makefile"
"-l s Makefile s21_grep.c func_grep.c"
"-n s Makefile"
"-o s21 Makefile"
"-h grep Makefile func_grep.c"
"grep -s 123123"
"-in S21 Makefile"
"-iv S Makefile"
"-lv s Makefile s21_grep.c func_grep.c"
"-ho grep Makefile s21_grep.c"
"-n for func_grep.c func_grep.c"
"-n for func_grep.c"
"-n -e ^\} func_grep.c"
"-c -e while func_grep.c"
"-ce int func_grep.c"
"-e ^int func_grep.c"
"-nivh = func_grep.c s21_grep.c"
"-e"
"-ie INT func_grep.c"
"-echar func_grep.c"
"-iv int func_grep.c"
"-in int func_grep.c"
"-v func_grep.c -e ank"
"-ne ) func_grep.c"
"-l for func_grep.c func_grep.c"
"-o -e int func_grep.c"
"-l for func_grep.c no_file.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo -en "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t\n"
    else
      (( FAIL++ ))
      echo -en "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t\n"
    fi
    rm test_s21_grep.log test_sys_grep.log
}


for i in "${extra[@]}"
do
    var="-"
    testing $i
done


echo -en "\033[31mFAIL: $FAIL\033[0m\n"
echo -en "\033[32mSUCCESS: $SUCCESS\033[0m\n"
echo -en "ALL: $COUNTER\n"
