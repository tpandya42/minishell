#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
BLUE='\033[0;34m'
NC='\033[0m'
ERROR="❌"
SUCCESS="✅"

run_test() {
    local description="$1"
    local cmd="$2"
    local expected="$3"

    echo -e "\n$BLUE=== $description ===$NC"
    output=$(eval "$cmd" 2>&1)
    status=$?

    echo "Command: $cmd"
    echo "Output:"
    echo "$output"

    if [ $status -eq 0 ]; then
        echo -e "$SUCCESS Exit code 0"
    else
        echo -e "$ERROR Exit code $status"
    fi
}

# ================= BUILTINS =================
echo -e "\n$BLUE=== BUILTINS TESTS ===$NC"
run_test "echo simple" "echo hello"
run_test "echo no newline" "echo -n hello"
run_test "pwd builtin" "pwd"
run_test "cd builtin" "cd / && pwd"
run_test "export and echo" "export TEST_VAR=123 && echo \$TEST_VAR"
run_test "unset variable" "unset TEST_VAR && echo \$TEST_VAR"
#run_test "env builtin" "env | grep TEST_VAR"

# ================= EXTERNAL COMMANDS =================
echo -e "\n$BLUE=== EXTERNAL COMMANDS ===$NC"
run_test "ls simple" "ls"
run_test "ls redirect" "ls > file1"
run_test "ls pipe + redirect" "ls | ls > file2"
#run_test "ls multiple pipes + grep" "ls | ls | ls | grep doc"
run_test "cat chained" "cat | cat | ls"
#run_test "env piped" "env | ls | ls | grep doc"

# ================= REDIRECTIONS =================
echo -e "\n$BLUE=== REDIRECTIONS ===$NC"
run_test "pwd redirect" "pwd > file"
run_test "env redirect" "env > file"
run_test "env redirect + pipe" "env > file | ls"
run_test "pwd redirect + pipe" "pwd > file | ls"
# Redir in
run_test 'ls > file'          # external
run_test 'echo hello > file'  # builtin

# Redir out
run_test 'cat < file'         # external

# With append
run_test 'date >> file'       # external
run_test 'env >> file'        # builtin

# ================= PIPES =================
echo -e "\n$BLUE=== PIPES ===$NC"
run_test "echo pipe" "echo hello | echo world"
run_test "echo redirect + pipe" "echo hello > file | echo world"
run_test "echo input + output redirection pipe" "echo hello < file1 | echo world > file2"
# run_test 'echo hello | grep o'
# run_test 'pwd > file | ls'
# run_test 'env > file | ls'
# run_test 'ls | echo hello | grep o'
# run_test 'env > file1 | env > file2'
# run_test 'pwd > file3 | env > file4'

echo -e "\n$GREEN===== ALL TESTS FINISHED =====$NC"
