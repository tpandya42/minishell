#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color
ERROR="❌"
SUCCESS="✅"

run_test() {
    echo -e "🔍 Running test: \033[1m$1\033[0m"
    bash "$1"
    if [ $? -ne 0 ]; then
        echo -e "${RED}${ERROR} Test $1 failed${NC}"
    else
        echo -e "${GREEN}${SUCCESS} Test $1 passed${NC}"
    fi
}

# This definition is to execute the scripts located in the test/ 
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Script list that we want to run in the main
# run_test "$SCRIPT_DIR/test_redir.sh"
# run_test "$SCRIPT_DIR/test_builtin.sh"
# run_test "$SCRIPT_DIR/execution_test.sh"
# Add more test scripts here
