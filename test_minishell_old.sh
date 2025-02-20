#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Counter for tests
PASSED=0
FAILED=0
TOTAL=0

# Function to print test results
print_result() {
    local test_name=$1
    local result=$2
    local expected=$3
    local actual=$4
    
    ((TOTAL++))
    
    if [ "$result" = "pass" ]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        ((PASSED++))
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo -e "Expected: $expected"
        echo -e "Got:      $actual"
        ((FAILED++))
    fi
}

# Function to compare command output with bash
compare_with_bash() {
    local cmd=$1
    local bash_output
    local minishell_output
    
    # Get bash output and exit code
    bash_output=$(echo "$cmd" | bash 2>&1)
    bash_exit=$?
    
    # Get minishell output and exit code
    minishell_output=$(echo "$cmd" | ./minishell 2>&1)
    minishell_exit=$?
    
    if [ "$bash_output" = "$minishell_output" ] && [ $bash_exit -eq $minishell_exit ]; then
        print_result "Command: $cmd" "pass" "" ""
    else
        print_result "Command: $cmd" "fail" "$bash_output (exit: $bash_exit)" "$minishell_output (exit: $minishell_exit)"
    fi
}

echo -e "${YELLOW}Starting Minishell tests...${NC}\n"

# 1. Basic Command Execution Tests
echo -e "\n${YELLOW}1. Basic Command Execution Tests${NC}"
compare_with_bash "ls"
compare_with_bash "ls -l"
compare_with_bash "/bin/ls"
compare_with_bash "./non_existent_command"

# 2. Quote Handling Tests
echo -e "\n${YELLOW}2. Quote Handling Tests${NC}"
compare_with_bash "echo 'single quotes'"
compare_with_bash "echo \"double quotes\""
compare_with_bash "echo 'mixed \"quotes\"'"
compare_with_bash "echo \"$HOME\""
compare_with_bash "echo '$HOME'"

# 3. Environment Variable Tests
echo -e "\n${YELLOW}3. Environment Variable Tests${NC}"
compare_with_bash "echo \$HOME"
compare_with_bash "echo \$PATH"
compare_with_bash "echo \$?"

# 4. Pipe Tests
echo -e "\n${YELLOW}4. Pipe Tests${NC}"
compare_with_bash "ls | grep a"
compare_with_bash "ls | wc -l"
compare_with_bash "cat /etc/passwd | grep root | wc -l"
compare_with_bash "ls | sort | head -n 3"

# 5. Redirection Tests
echo -e "\n${YELLOW}5. Redirection Tests${NC}"
compare_with_bash "ls > output.txt"
compare_with_bash "cat < output.txt"
compare_with_bash "ls >> output.txt"
compare_with_bash "cat << EOF
test
EOF"

# 6. Built-in Commands Tests
echo -e "\n${YELLOW}6. Built-in Commands Tests${NC}"

# echo tests
compare_with_bash "echo hello world"
compare_with_bash "echo -n hello world"
compare_with_bash "echo \"hello    world\""

# cd tests
test_cd() {
    local init_dir=$(pwd)
    echo "cd /tmp" | ./minishell
    local new_dir=$(pwd)
    cd $init_dir
    if [ "$new_dir" = "/tmp" ]; then
        print_result "cd /tmp" "pass" "" ""
    else
        print_result "cd /tmp" "fail" "/tmp" "$new_dir"
    fi
}
test_cd

# pwd test
compare_with_bash "pwd"

# export and env tests
test_export() {
    echo "export TEST=123" | ./minishell
    echo "env | grep ^TEST=" | ./minishell > minishell_output
    echo "export TEST=123" | bash
    echo "env | grep ^TEST=" | bash > bash_output
    
    if diff minishell_output bash_output >/dev/null 2>&1; then
        print_result "export TEST=123" "pass" "" ""
    else
        print_result "export TEST=123" "fail" "$(cat bash_output)" "$(cat minishell_output)"
    fi
    rm -f minishell_output bash_output
}
test_export

# unset test
test_unset() {
    echo -e "export TEST=123\nunset TEST\nenv | grep ^TEST=" | ./minishell > minishell_output
    echo -e "export TEST=123\nunset TEST\nenv | grep ^TEST=" | bash > bash_output
    
    if diff minishell_output bash_output >/dev/null 2>&1; then
        print_result "unset TEST" "pass" "" ""
    else
        print_result "unset TEST" "fail" "$(cat bash_output)" "$(cat minishell_output)"
    fi
    rm -f minishell_output bash_output
}
test_unset

# Exit test
test_exit() {
    echo "exit 42" | ./minishell
    local minishell_exit=$?
    if [ $minishell_exit -eq 42 ]; then
        print_result "exit 42" "pass" "" ""
    else
        print_result "exit 42" "fail" "42" "$minishell_exit"
    fi
}
test_exit

# 7. Signal Handling Tests
echo -e "\n${YELLOW}7. Signal Handling Tests${NC}"
echo "Note: Manual testing required for Ctrl+C, Ctrl+D, and Ctrl+\\"
echo "- Ctrl+C should display a new prompt on a new line"
echo "- Ctrl+D should exit the shell"
echo "- Ctrl+\\ should do nothing"

# Memory leak test reminder
echo -e "\n${YELLOW}Memory Leak Tests${NC}"
echo "Run your minishell under valgrind to check for memory leaks:"
echo "valgrind --leak-check=full --show-leak-kinds=all ./minishell"

# Print final results
echo -e "\n${YELLOW}Test Results:${NC}"
echo "Total tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

# Cleanup
rm -f output.txt

exit $([ $FAILED -eq 0 ])