#!/bin/bash

# Test script for minishell evaluation criteria
# This script tests the mandatory requirements from the correction sheet

echo "Testing Minishell Implementation"
echo "================================="

# Test 1: Simple commands
echo "Test 1: Simple commands"
echo "/bin/ls"
echo "/bin/pwd"
echo ""

# Test 2: Commands with arguments  
echo "Test 2: Commands with arguments"
echo "/bin/ls -la"
echo "/bin/echo hello world"
echo ""

# Test 3: Built-in commands
echo "Test 3: Built-in commands"
echo "echo test"
echo "echo -n test"
echo "pwd"
echo "env"
echo "export TEST=value"
echo "unset TEST"
echo "cd /"
echo "cd .."
echo ""

# Test 4: Environment variables
echo "Test 4: Environment variables"
echo "echo \$HOME"
echo "echo \$USER"
echo "echo \$?"
echo ""

# Test 5: Quotes
echo "Test 5: Quote handling"
echo "echo \"hello world\""
echo "echo 'hello world'"
echo "echo '\$USER'"
echo "echo \"\$USER\""
echo ""

# Test 6: Redirections
echo "Test 6: Redirections"
echo "echo test > output.txt"
echo "cat < output.txt"
echo "echo test2 >> output.txt"
echo "cat << EOF"
echo "test"
echo "EOF"
echo ""

# Test 7: Pipes
echo "Test 7: Pipes"
echo "ls | grep test"
echo "echo hello | cat"
echo "cat /etc/passwd | head -5 | tail -2"
echo ""

# Test 8: Signal handling
echo "Test 8: Signal handling (manual test)"
echo "- Press Ctrl-C during empty prompt"
echo "- Press Ctrl-D during empty prompt" 
echo "- Press Ctrl-\\ during empty prompt"
echo "- Try signals during command execution"
echo ""

echo "Run: ./minishell"
echo "Then test these commands manually"