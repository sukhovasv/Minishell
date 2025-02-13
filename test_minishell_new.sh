#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'

# Counters
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
        echo -e "Expected:\n$expected"
        echo -e "Got:\n$actual"
        ((FAILED++))
    fi
}

# Function to compare output, ignoring DEBUG messages
compare_with_bash() {
    local cmd="$1"
    
    # Get bash output (filtered)
    bash_output=$(echo "$cmd" | bash 2>&1 | grep -v "DEBUG:")
    bash_exit=$?

    # Get minishell output (filtered)
    minishell_output=$(echo "$cmd" | ./minishell 2>&1 | grep -v "DEBUG:")
    minishell_exit=$?

    if [ "$bash_output" = "$minishell_output" ] && [ $bash_exit -eq $minishell_exit ]; then
        print_result "Command: $cmd" "pass" "" ""
    else
        print_result "Command: $cmd" "fail" "$bash_output" "$minishell_output"
    fi
}

# TEST CASES

# 🟢 Базовые команды
compare_with_bash "echo hello"
compare_with_bash "ls -l"
compare_with_bash "pwd"

# 🟢 Переменные окружения (без `&&`)
compare_with_bash "export TESTVAR=42"
compare_with_bash "echo \$TESTVAR"
compare_with_bash "unset TESTVAR"
compare_with_bash "echo \$TESTVAR"

# 🟢 Специальные переменные
compare_with_bash "echo \$?"
compare_with_bash "echo \$HOME"
compare_with_bash "echo \$USER"

# 🟢 Простые пайпы
compare_with_bash "echo hello | grep hello"
compare_with_bash "ls | wc -l"

# 🟢 Простые heredoc
compare_with_bash "cat << EOF
hello
world
EOF"

compare_with_bash "cat << EOF | grep hello
hello
world
EOF"

# 🟢 Heredoc + редиректы
compare_with_bash "cat << EOF > testfile
hello
world
EOF
cat testfile"

compare_with_bash "cat << EOF >> testfile
append
EOF
cat testfile"

compare_with_bash "rm testfile"

# 🟢 Heredoc + пайпы
compare_with_bash "cat << EOF | grep hello | wc -l
hello
world
hello
EOF"

compare_with_bash "cat << EOF | sort | uniq | wc -l
banana
apple
banana
orange
EOF"

# 🟢 Несколько `heredoc` в одной команде
compare_with_bash "cat << EOF1 | grep hello | cat << EOF2 | wc -l
hello
EOF1
world
EOF2"

# 🟢 Пайпы с `heredoc` и редиректами
compare_with_bash "cat << EOF | grep hello | tee output.txt | cat
hello
world
hello
EOF"

compare_with_bash "rm output.txt"

# 🟢 Несколько пайпов с heredoc (ключевой тест!)
compare_with_bash "cat << EOF | grep hello | sort | uniq | wc -l
hello
banana
world
hello
EOF"

compare_with_bash "cat << EOF | grep hello | grep world | wc -l
hello world
hello
world
EOF"

# 🟢 Builtins (cd, export, unset, env, exit)
compare_with_bash "cd /tmp && pwd"
compare_with_bash "env | grep PATH"
compare_with_bash "exit 42"

# 🟢 Вложенные minishell
compare_with_bash "./minishell -c 'echo nested'"

# 🔴 Ошибки: пайп в начале
compare_with_bash "| echo error"

# 🔴 Ошибки: незакрытая кавычка
compare_with_bash "echo 'Unclosed quote"

# 🔴 Ошибки: cat < несуществующий файл
compare_with_bash "cat < non_existent_file"

# 🔴 Ошибки: heredoc без EOF
compare_with_bash "<< EOF | wc -l
hello
EOF"

# 🟢 Обработка `Ctrl-C`, `Ctrl-D`, `Ctrl-\`
echo -e "\n${YELLOW}!!! Для ручной проверки запусти minishell и попробуй:${NC}"
echo -e "${YELLOW}  1. Нажми Ctrl-C → должен появиться новый промпт.${NC}"
echo -e "${YELLOW}  2. Нажми Ctrl-D → minishell должен завершиться.${NC}"
echo -e "${YELLOW}  3. Нажми Ctrl-\\ → должно ничего не произойти.${NC}"

# 🟡 Итог
echo -e "\n${YELLOW}Total: $TOTAL | Passed: $PASSED | Failed: $FAILED${NC}"
