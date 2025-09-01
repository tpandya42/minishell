#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'
ERROR="❌"
SUCCESS="✅"

# Counters
total_tests=0
passed_tests=0

run_test() {
    local cmd="$1"
    local desc="$2"
    total_tests=$((total_tests + 1))
    
    echo -e "\n$BLUE=== $desc ($cmd) ===$NC"

    # Get exit code from Bash
    bash -c "$cmd" >/dev/null 2>&1
    bash_status=$?

    # Execute in current shell
    eval "$cmd"
    shell_status=$?

    # Compare exit codes
    if [ "$shell_status" -eq "$bash_status" ]; then
        echo -e "$SUCCESS Exit code matches Bash ($shell_status)"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "$ERROR Exit code differs! Shell: $shell_status, Bash: $bash_status"
    fi
}

echo -e "$BLUE===== BUILTINS TESTS =====$NC"

# ================== ECHO TESTS ==================
echo -e "$BLUE--- ECHO ---$NC"
run_test 'echo hello world' 'echo hello world'
run_test 'echo "hello world"' 'echo "hello world"'
run_test "echo 'hello world'" "echo 'hello world'"
run_test "echo hello'world'" "echo hello'world'"
run_test 'echo hello""world' 'echo hello""world'
run_test "echo ''" "echo ''"
run_test 'echo "$PWD"' 'echo "$PWD"'
run_test "echo '\$PWD'" "echo '\$PWD'"
run_test "echo \"aspas ->'\"" "echo \"aspas ->'\""
run_test 'echo "aspas -> '\'' "' 'echo "aspas -> '\'' "'
run_test 'echo '\''aspas ->"'\''' 'echo '\''aspas ->"'\'''
run_test 'echo '\''aspas -> " '\'' 'echo '\''aspas -> " '\'' '
run_test 'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $ \ <<"' 'echo "> >> < * ? [ ] | ; [ ] || && ( ) & # $ \ <<"'
run_test "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # $ \ <<'" "echo '> >> < * ? [ ] | ; [ ] || && ( ) & # $ \ <<'"
run_test 'echo "exit_code ->$? user ->$USER home -> $HOME"' 'echo "exit_code ->$? user ->$USER home -> $HOME"'
run_test "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'" "echo 'exit_code ->\$? user ->\$USER home -> \$HOME'"
run_test 'echo "$"' 'echo "$"'
run_test "echo '\$'" "echo '\$'"
run_test 'echo $' 'echo $'
run_test 'echo $?' 'echo $?'
run_test 'echo $?HELLO' 'echo $?HELLO'

# ================== PWD / CD TESTS ==================
echo -e "$BLUE--- PWD / CD ---$NC"
run_test 'pwd' 'pwd'
run_test 'pwd oi' 'pwd oi'
run_test 'cd $PWD' 'cd $PWD'
run_test 'cd $PWD hi' 'cd $PWD hi'
run_test 'cd 123123' 'cd 123123'

# ================== EXPORT TESTS ==================
echo -e "$BLUE--- EXPORT ---$NC"
run_test 'export hello' 'export hello'
run_test 'export HELLO=123' 'export HELLO=123'
run_test 'export A-' 'export A-'
run_test 'export HELLO=123 A' 'export HELLO=123 A'
run_test 'export HELLO="123 A-"' 'export HELLO="123 A-"'
run_test 'export hello world' 'export hello world'
run_test 'export HELLO-=123' 'export HELLO-=123'
run_test 'export =' 'export ='
run_test 'export 123' 'export 123'

# ================== UNSET TESTS ==================
echo -e "$BLUE--- UNSET ---$NC"
run_test 'unset' 'unset'
run_test 'unset HELLO' 'unset HELLO'
run_test 'unset HELLO1 HELLO2' 'unset HELLO1 HELLO2'
run_test 'unset HOME' 'unset HOME'
run_test 'unset PATH' 'unset PATH'
run_test 'unset SHELL' 'unset SHELL'

# ================== EXIT TESTS ==================
echo -e "$BLUE--- EXIT ---$NC"
run_test 'exit 123' 'exit 123'
run_test 'exit 298' 'exit 298'
run_test 'exit +100' 'exit +100'
run_test 'exit "+100"' 'exit "+100"'
run_test 'exit +"100"' 'exit +"100"'
run_test 'exit -100' 'exit -100'
run_test 'exit "-100"' 'exit "-100"'
run_test 'exit -"100"' 'exit -"100"'
run_test 'exit hello' 'exit hello'
run_test 'exit 42 world' 'exit 42 world'

# ================= SUMMARY =================
echo -e "\n$BLUE===== SUMMARY =====$NC"
echo "Passed $passed_tests / $total_tests tests"

if [ $passed_tests -eq $total_tests ]; then
    echo -e "$GREEN All tests passed! $SUCCESS$NC"
else
    echo -e "$RED Some tests failed! $ERROR$NC"
fi

echo -e "$GREEN===== BUILTINS TESTS FINISHED =====$NC"
