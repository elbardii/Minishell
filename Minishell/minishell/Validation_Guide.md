# Minishell Testing & Validation Guide

A comprehensive testing framework and validation checklist for the Minishell project based on the 42 School evaluation criteria.

## Table of Contents

- [Overview](#overview)
- [Pre-Evaluation Setup](#pre-evaluation-setup)
- [Testing Categories](#testing-categories)
- [Automated Testing Scripts](#automated-testing-scripts)
- [Manual Testing Procedures](#manual-testing-procedures)
- [Common Issues & Debugging](#common-issues--debugging)
- [Evaluation Checklist](#evaluation-checklist)

## Overview

This guide provides systematic testing procedures to ensure your Minishell implementation meets all requirements and handles edge cases correctly. The tests are organized by functionality and include both automated scripts and manual testing procedures.

### Evaluation Principles
- **No crashes or unexpected terminations**
- **Proper memory management (no leaks)**
- **Norm compliance**
- **Bash-compatible behavior**
- **Robust error handling**

## Pre-Evaluation Setup

### Repository Verification
```bash
# Clone in clean directory
mkdir test_minishell && cd test_minishell
git clone <repository_url> .

# Verify repository authenticity
git log --oneline -5
git status
```

### Compilation Tests
```bash
# Check compilation flags
make -n | grep -E "(Wall|Wextra|Werror)"

# Test clean compilation
make fclean && make
echo $?  # Should be 0

# Test no relinking
make
make  # Should not recompile
```

### Memory Leak Setup
```bash
# Install valgrind (if not available)
# Ubuntu/Debian: sudo apt-get install valgrind
# macOS: brew install valgrind (if available) or use leaks

# Memory leak check function
alias check_leaks='valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes'
# On macOS: alias check_leaks='leaks -atExit --'
```

## Testing Categories

## 1. Basic Functionality Tests

### Simple Commands
```bash
# Test absolute paths
./minishell
/bin/ls
/usr/bin/env
/bin/pwd
exit

# Test with arguments
./minishell
/bin/ls -la
/bin/echo "hello world"
/bin/cat /etc/passwd
exit
```

### Global Variables Check
**Manual verification required:**
- Count global variables in the code
- Verify purpose (should only store signal numbers)
- Ensure no additional data access through globals

### Empty Input Handling
```bash
./minishell
# Press Enter (empty command)
    # Press Enter (spaces/tabs only)
		# Press Enter (mixed whitespace)
exit
```

## 2. Built-in Commands Tests

### Echo Command
```bash
./minishell
echo
echo "hello"
echo hello world
echo -n "no newline"
echo -n
echo -n "test" "multiple" "args"
exit
```

### Exit Command
```bash
# Test exit variations
./minishell
exit
# Relaunch
./minishell
exit 0
# Relaunch
./minishell
exit 42
# Relaunch
./minishell
exit invalid_arg
# Should show error and exit with code 2
```

### Environment Commands
```bash
./minishell
env
export TEST_VAR="hello"
env | grep TEST_VAR
unset TEST_VAR
env | grep TEST_VAR
export USER="testuser"
echo $USER
exit
```

### Directory Commands
```bash
./minishell
pwd
cd /tmp
pwd
cd ..
pwd
cd .
pwd
cd nonexistent_dir  # Should fail
pwd
cd ~  # If HOME is set
pwd
exit
```

## 3. Advanced Feature Tests

### Exit Status Testing
```bash
./minishell
/bin/true
echo $?  # Should print 0
/bin/false
echo $?  # Should print 1
/bin/ls nonexistent_file
echo $?  # Should print 2 or similar
expr $? + $?  # Should calculate properly
exit
```

### Signal Handling Tests
**Interactive testing required:**

1. **Ctrl-C on empty prompt:**
   - Should show new line with new prompt
   - Buffer should be clean

2. **Ctrl-\ on empty prompt:**
   - Should do nothing

3. **Ctrl-D on empty prompt:**
   - Should exit minishell

4. **Ctrl-C after typing:**
   - Type some text but don't press Enter
   - Press Ctrl-C
   - Should show new prompt, clean buffer
   - Press Enter - nothing should execute

5. **Ctrl-D/Ctrl-\ after typing:**
   - Should not affect the prompt

6. **Signals during blocking commands:**
   ```bash
   ./minishell
   cat  # No arguments - will block
   # Try Ctrl-C, Ctrl-\, Ctrl-D
   # Try with grep "something", sleep 10, etc.
   ```

## 4. Quote Handling Tests

### Double Quotes
```bash
./minishell
echo "hello world"
echo "cat lol.c | cat > lol.c"  # Should not execute pipe
echo "   spaces   preserved   "
echo "$USER is the user"  # Variable expansion
echo "\$USER"  # Escaped dollar
exit
```

### Single Quotes
```bash
./minishell
echo 'hello world'
echo '$USER'  # Should print literal $USER
echo '   spaces   '
echo 'cat | grep | more'  # All literal
echo ''  # Empty quotes
exit
```

## 5. Redirection Tests

### Basic Redirections
```bash
./minishell
echo "test" > output.txt
cat < output.txt
echo "append" >> output.txt
cat output.txt
rm output.txt
exit
```

### Heredoc Testing
```bash
./minishell
cat << EOF
line1
line2
EOF
wc -l << DELIMITER
first line
second line
DELIMITER
exit
```

### Complex Redirections
```bash
./minishell
echo "test" > file1 > file2  # Multiple redirections
cat < nonexistent_file  # Should fail gracefully
echo "test" >> newfile
rm file1 file2 newfile 2>/dev/null
exit
```

## 6. Pipe Tests

### Simple Pipes
```bash
./minishell
echo "hello" | cat
ls | grep minishell
cat /etc/passwd | head -5 | tail -2
exit
```

### Error Handling in Pipes
```bash
./minishell
ls nonexistent | grep something
cat nonexistent_file | wc -l
ls | nonexistent_command | wc
exit
```

### Mixed Pipes and Redirections
```bash
./minishell
echo "test" | cat > output.txt
cat < /etc/passwd | grep root > users.txt
rm output.txt users.txt 2>/dev/null
exit
```

## 7. Path Resolution Tests

### Relative Paths
```bash
./minishell
./minishell  # If executable
../minishell  # From subdirectory
cd /bin && ./ls  # Should work
exit
```

### PATH Variable Tests
```bash
./minishell
ls  # Should work with PATH
echo $PATH
export OLD_PATH="$PATH"
unset PATH
ls  # Should fail
export PATH="$OLD_PATH"
ls  # Should work again
export PATH="/bin:/usr/bin"
ls  # Should work
export PATH="/usr/bin:/bin"  # Different order
which ls  # If available, check path resolution
exit
```

## 8. Environment Variable Tests

### Variable Expansion
```bash
./minishell
echo $USER
echo $HOME
echo $PATH
echo $NONEXISTENT  # Should expand to empty
echo "$USER"  # In double quotes
echo '$USER'  # In single quotes - should be literal
exit
```

### Complex Variable Tests
```bash
./minishell
export TEST="hello"
echo $TEST
echo "$TEST world"
echo ${TEST}  # If supported
echo $TEST$TEST  # Concatenation
exit
```

## Automated Testing Scripts

### Basic Test Script
```bash
#!/bin/bash
# basic_tests.sh

MINISHELL="./minishell"
TESTS_PASSED=0
TESTS_TOTAL=0

test_command() {
    local cmd="$1"
    local expected="$2"
    local description="$3"
    
    echo "Testing: $description"
    echo "$cmd" | timeout 5 $MINISHELL > test_output.tmp 2>&1
    
    if [ $? -eq 124 ]; then
        echo "❌ TIMEOUT: $description"
        return 1
    fi
    
    if grep -q "$expected" test_output.tmp; then
        echo "✅ PASSED: $description"
        ((TESTS_PASSED++))
    else
        echo "❌ FAILED: $description"
        echo "Expected: $expected"
        echo "Got: $(cat test_output.tmp)"
    fi
    
    ((TESTS_TOTAL++))
    rm -f test_output.tmp
}

# Run basic tests
test_command "echo hello" "hello" "Basic echo"
test_command "pwd" "/" "PWD command"
test_command "/bin/ls" "minishell" "Absolute path command"

echo "Tests passed: $TESTS_PASSED/$TESTS_TOTAL"
```

### Memory Leak Test Script
```bash
#!/bin/bash
# memory_test.sh

MINISHELL="./minishell"
VALGRIND="valgrind --leak-check=full --error-exitcode=1"

echo "Running memory leak tests..."

# Test basic commands
echo "echo hello\nexit" | $VALGRIND $MINISHELL
if [ $? -eq 0 ]; then
    echo "✅ No memory leaks in basic commands"
else
    echo "❌ Memory leaks detected in basic commands"
fi

# Test complex operations
echo "ls | grep minishell\nexit" | $VALGRIND $MINISHELL
if [ $? -eq 0 ]; then
    echo "✅ No memory leaks in pipes"
else
    echo "❌ Memory leaks detected in pipes"
fi
```

## Manual Testing Procedures

### Stress Testing
```bash
# Long command line
./minishell
echo "very long argument with many words to test argument parsing and memory allocation handling in various scenarios with multiple parameters and complex structures"

# Many arguments
echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10 arg11 arg12 arg13 arg14 arg15

# Complex pipes
cat /etc/passwd | grep root | cut -d: -f1 | tr 'a-z' 'A-Z' | wc -c

# Mixed operations
echo "test" > file1 && cat file1 | grep test > file2 && rm file1 file2
```

### History Testing
1. Type several commands
2. Use Up/Down arrows to navigate
3. Test Ctrl-C and then Up arrow
4. Verify history persistence

### Edge Cases
```bash
./minishell
# Test empty pipe components
| ls  # Should handle gracefully
ls |  # Should handle gracefully
ls | | cat  # Should handle gracefully

# Test invalid commands
nonexistent_command
./nonexistent_file
/nonexistent/path

# Test special characters
echo "test with \$HOME and \$USER"
echo 'literal \$HOME and \$USER'
```

## Common Issues & Debugging

### Compilation Issues
- **Relinking**: Check Makefile dependencies
- **Flags missing**: Verify CFLAGS in Makefile
- **Library linking**: Ensure readline is properly linked

### Runtime Issues
- **Segmentation faults**: Use gdb or add debug prints
- **Memory leaks**: Use valgrind systematically
- **Signal handling**: Test in various scenarios

### Behavior Issues
- **Quote parsing**: Test nested and mixed quotes
- **Variable expansion**: Test edge cases and special variables
- **Path resolution**: Test with modified PATH

## Evaluation Checklist

### Pre-Evaluation
- [ ] Repository is clean and authentic
- [ ] Compilation works without errors
- [ ] No unnecessary relinking occurs
- [ ] Global variables are minimal and justified

### Mandatory Features
- [ ] Simple commands with absolute paths work
- [ ] Commands with arguments work
- [ ] Built-in echo works (with -n option)
- [ ] Built-in exit works (with arguments)
- [ ] Exit status ($?) works correctly
- [ ] Signal handling (Ctrl-C, Ctrl-D, Ctrl-\) works
- [ ] Double quotes work (with variable expansion)
- [ ] Single quotes work (no expansion)
- [ ] env, export, unset work
- [ ] cd and pwd work
- [ ] Relative paths work
- [ ] PATH resolution works
- [ ] Basic redirections (<, >, >>) work
- [ ] Heredoc (<<) works
- [ ] Pipes work
- [ ] Environment variables expand correctly

### Quality Checks
- [ ] No memory leaks detected
- [ ] No crashes or unexpected termination
- [ ] Error messages are appropriate
- [ ] Complex command combinations work
- [ ] History functionality works

### Edge Cases
- [ ] Empty commands handled gracefully
- [ ] Invalid commands show proper errors
- [ ] Long command lines work
- [ ] Complex pipe chains work
- [ ] Mixed redirections and pipes work

## Testing Commands Summary

```bash
# Quick validation script
#!/bin/bash
make && {
    echo "=== COMPILATION OK ==="
    echo "echo hello\npwd\n/bin/ls\nexit" | ./minishell
    echo "=== MEMORY TEST ==="
    echo "echo test\nexit" | valgrind --leak-check=full ./minishell
    echo "=== SIGNAL TEST (manual) ==="
    echo "Now test Ctrl-C, Ctrl-D manually..."
    ./minishell
}
```

---

## Notes for Evaluators

- Always test on a clean repository clone
- Use both automated and manual testing approaches  
- Pay attention to bash compatibility
- Document any deviations or issues found
- Test edge cases thoroughly
- Verify memory management with appropriate tools

Remember: The goal is to ensure the minishell behaves like a real shell while maintaining code quality and educational value.