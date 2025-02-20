#!/bin/bash

# Цвета для вывода
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m'

# Путь к minishell
MINISHELL="./minishell"

echo -e "${BLUE}Starting Minishell Tests${NC}\n"

# Функция для запуска теста
run_test() {
    local test_name=$1
    local command=$2
    
    echo -e "\n${BLUE}Testing: ${test_name}${NC}"
    echo "Command: $command"
    
    # Запускаем команду в bash
    echo -e "\n${BLUE}Expected output (bash):${NC}"
    eval "$command"
    local bash_status=$?
    
    # Запускаем команду в minishell
    echo -e "\n${BLUE}Your minishell output:${NC}"
    echo "$command" | $MINISHELL
    local minishell_status=$?
    
    # Сравниваем статусы выхода
    if [ $bash_status -eq $minishell_status ]; then
        echo -e "${GREEN}Status check: OK${NC}"
    else
        echo -e "${RED}Status check: KO (bash: $bash_status, minishell: $minishell_status)${NC}"
    fi
    
    echo "----------------------------------------"
}

# Базовые тесты
echo -e "${BLUE}\nBasic Tests:${NC}"
run_test "Simple echo" 'echo Hello World'
run_test "Echo with variable" 'echo $USER'
run_test "Echo with quoted variable" 'echo "$USER"'
run_test "Echo with single quotes" 'echo '\''$USER'\'''
run_test "Echo multiple variables" 'echo "$USER$HOME"'

# Тесты редиректов
echo -e "${BLUE}\nRedirection Tests:${NC}"
run_test "Output redirection" 'echo "hello" > file1.txt && cat file1.txt'
run_test "Input redirection" 'echo "test" > infile.txt && cat < infile.txt'
run_test "Append redirection" 'echo "world" >> file1.txt && cat file1.txt'
run_test "Multiple redirections" 'echo "test" > file1.txt > file2.txt && cat file2.txt'

# Тесты heredoc
echo -e "${BLUE}\nHeredoc Tests:${NC}"
run_test "Simple heredoc" 'cat << EOF
Hello
$USER
EOF'

run_test "Quoted heredoc" 'cat << "EOF"
Hello
$USER
EOF'

# Тесты пайпов
echo -e "${BLUE}\nPipe Tests:${NC}"
run_test "Simple pipe" 'echo "Hello World" | grep "Hello"'
run_test "Multiple pipes" 'ls -l | grep "." | wc -l'
run_test "Complex pipes" 'cat /etc/passwd | grep "root" | wc -l'

# Тесты с комбинациями
echo -e "${BLUE}\nCombination Tests:${NC}"
run_test "Heredoc with pipe" 'cat << EOF | grep "hello"
hello world
goodbye world
EOF'

run_test "Pipe with redirections" 'ls -l | grep "." > output.txt && cat output.txt'

# Тесты встроенных команд
echo -e "${BLUE}\nBuiltin Tests:${NC}"
run_test "pwd command" 'pwd'
run_test "echo exit status" 'echo $?'
run_test "export and echo" 'export TEST=value && echo $TEST'
run_test "unset" 'export TEST=value && unset TEST && echo $TEST'

# Тесты на ошибки
echo -e "${BLUE}\nError Handling Tests:${NC}"
run_test "Nonexistent file" 'cat nonexistentfile'
run_test "Permission denied" 'echo "test" > /root/file'

# Очистка временных файлов
rm -f file1.txt file2.txt infile.txt output.txt

echo -e "\n${BLUE}All tests completed${NC}"
