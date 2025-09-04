#!/bin/bash

# Test runner for the 42 minishell project.
# It compares the output and exit codes of your minishell against bash.

# --- CONFIGURATION ---
MINISHELL_EXEC="./minishell"

# --- COLORS ---
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# --- COUNTERS ---
total_tests=0
passed_tests=0

# --- SETUP ---
# Ensure the minishell executable exists and is executable
if [ ! -f "$MINISHELL_EXEC" ]; then
    echo -e "${RED}Error: Minishell executable not found at '$MINISHELL_EXEC'${NC}"
    echo "Please compile your project first (e.g., 'make')."
    exit 1
fi
if [ ! -x "$MINISHELL_EXEC" ]; then
    echo -e "${RED}Error: Minishell is not executable.${NC}"
    echo "Please run 'chmod +x $MINISHELL_EXEC'."
    exit 1
fi

# Create a temporary directory for test outputs that will be cleaned up on exit
TEST_DIR=$(mktemp -d)
trap 'rm -rf -- "$TEST_DIR"' EXIT

# --- TEST FUNCTION ---
# Usage: run_test "command to test" "description"
run_test() {
    local cmd_to_test=$1
    local description=$2
    total_tests=$((total_tests + 1))

    printf "🧪 Running test: ${YELLOW}%-40s${NC}" "$description"

    # Create temp files for outputs
    local m_stdout="$TEST_DIR/m_stdout.txt"
    local m_stderr="$TEST_DIR/m_stderr.txt"
    local b_stdout="$TEST_DIR/b_stdout.txt"
    local b_stderr="$TEST_DIR/b_stderr.txt"

    # Run in minishell. We use printf to send one or more commands.
    # Your shell will execute them sequentially and then exit on EOF.
    printf "%s\n" "$cmd_to_test" | "$MINISHELL_EXEC" > "$m_stdout" 2> "$m_stderr"
    local m_exit_code=$?

    # Run in bash for comparison. We replace newlines with semicolons for bash -c.
    local bash_cmd=$(echo -e "$cmd_to_test" | tr '\n' ';')
    bash -c "$bash_cmd" > "$b_stdout" 2> "$b_stderr"
    local b_exit_code=$?

    # --- SPECIAL HANDLING ---
    # Your `b_exit` builtin prints "exit" to stdout, but bash's `exit` does not.
    # If the last command in the sequence is `exit`, we add "exit" to bash's
    # expected output to make the test match your implementation.
    local last_cmd=$(echo -e "$cmd_to_test" | tail -n 1 | xargs)
    if [[ "$last_cmd" == exit* ]]; then
        # `exit` with too many args is a special case where your shell returns
        # but bash exits. This test will likely fail, but we handle the "happy path".
        if ! echo "$last_cmd" | grep -q 'exit .* .*'; then
             echo "exit" >> "$b_stdout"
       fi
    fi

    # For `env` and `export` (no args), sort the output to prevent failures
    # due to different environment variable ordering.
    if [[ "$last_cmd" == "env" || "$last_cmd" == "export" ]]; then
        sort "$m_stdout" -o "$m_stdout"
        sort "$b_stdout" -o "$b_stdout"
    fi

    # --- COMPARISON ---
    local diff_stdout=$(diff -u "$b_stdout" "$m_stdout")
    local diff_stderr=$(diff -u "$b_stderr" "$m_stderr")

    if [ "$diff_stdout" == "" ] && [ "$diff_stderr" == "" ] && [ "$m_exit_code" -eq "$b_exit_code" ]; then
        echo -e "[${GREEN}PASS${NC}]"
        passed_tests=$((passed_tests + 1))
    else
        echo -e "[${RED}FAIL${NC}]"
        if [ "$diff_stdout" != "" ]; then
            echo -e "${YELLOW}--- STDOUT Diff ---${NC} ((-) bash, (+) minishell)"
            echo "$diff_stdout"
        fi
        if [ "$diff_stderr" != "" ]; then
            echo -e "${YELLOW}--- STDERR Diff ---${NC} ((-) bash, (+) minishell)"
            echo "$diff_stderr"
        fi
        if [ "$m_exit_code" -ne "$b_exit_code" ]; then
            echo -e "${YELLOW}--- Exit Code Diff ---${NC}"
            echo "bash returned:      $b_exit_code"
            echo "minishell returned: $m_exit_code"
        fi
        echo "------------------------------------------------------"
    fi
}

# --- TEST CASES ---

echo "--- Testing Empty/Whitespace Input ---"
run_test "" "empty input"
run_test "   " "only whitespace"
run_test "		" "only tabs"

echo ""
echo "--- Testing 'echo' ---"
run_test "echo hello world" "simple echo"
run_test "echo -n hello" "echo with -n flag"
run_test "echo -n -n -n hello" "echo with multiple -n flags"
run_test "echo hello -n world" "echo with -n after text"
run_test "echo '  hello   world  '" "echo with single quotes"
run_test "echo \"  hello   world  \"" "echo with double quotes"
run_test "echo" "echo with no arguments"
run_test "echo ''" "echo with empty single quotes"
run_test "echo \"\"" "echo with empty double quotes"
run_test "echo -n" "echo with only -n flag"

echo ""
echo "--- Testing 'pwd' and 'cd' ---"
run_test "pwd" "pwd in current directory"
mkdir -p "$TEST_DIR/a/b"
run_test "cd '$TEST_DIR/a/b'
pwd" "cd to deep dir"
run_test "cd /
pwd" "cd to root and pwd"
run_test "cd" "cd with no arguments (to HOME)"
run_test "cd .
pwd" "cd to dot"
run_test "cd ../../..
pwd" "cd to parent dirs"
run_test "cd /tmp
cd /var
cd -
pwd" "cd -"
run_test "cd non_existent_directory" "cd to non-existent directory"
run_test "cd /etc/passwd" "cd to a file (should fail)"

echo ""
echo "--- Testing 'export', 'unset', and 'env' ---"
run_test "export" "export without arguments"
run_test "export VAR1=test
env" "export new variable"
run_test "export VAR2='hello world'
env" "export with spaces"
run_test "export VAR3
env" "export without value"
run_test "export VAR3=later
env" "assign value to var with no value"
run_test "export VAR4=old
export VAR4=new
env" "overwrite existing variable"
run_test "export _VAR=valid
env" "export with valid identifier"
run_test "export A=1 B=2 C=3
env" "export multiple variables"
run_test "export 9VAR=invalid" "export with invalid identifier"
run_test "export VAR+='invalid'" "export with invalid identifier (+)"
run_test "export MY_VAR=123
unset MY_VAR
env" "unset a variable"
run_test "unset NON_EXISTENT" "unset non-existent variable"
run_test "unset A B C
env" "unset multiple variables"
run_test "unset _VAR" "unset a valid _ variable"
run_test "unset 9VAR" "unset an invalid identifier"
run_test "env" "env command"
run_test "export HIDDEN
env" "env should not show exported vars without '='"
run_test "export HIDDEN
export" "export should show exported vars without '='"

echo ""
echo "--- Testing variable expansion ---"
run_test "export E_VAR=expansion_test
echo \$E_VAR" "simple expansion"
run_test "export E_VAR=expansion_test
echo 'S_VAR is \$E_VAR'" "no expansion in single quotes"
run_test "export E_VAR=expansion_test
echo \"D_VAR is \$E_VAR\"" "expansion in double quotes"
run_test "echo \$NON_EXISTENT_VAR" "expand non-existent var"
run_test "export EMPTY_VAR=
echo \"[\$EMPTY_VAR]\"" "expand empty var"
run_test "export A=Hello B=World
echo \$A\$B" "expand adjacent vars"
run_test "export A=Hello
echo \$A_WORLD" "expand var with trailing text"
run_test "echo \$" "echo single dollar sign"
run_test "echo \$=" "echo dollar followed by non-alnum"

echo ""
echo "--- Testing \$? (Last Exit Status) ---"
echo -e "${YELLOW}NOTE: These tests will likely fail until you implement \$? expansion.${NC}"
run_test "echo \$?
echo \$?" "initial exit status"
run_test "this_command_does_not_exist
echo \$?" "exit status of not found"
run_test "export
echo \$?" "exit status of successful builtin"
run_test "export 9A=
echo \$?" "exit status of failed builtin"

echo ""
echo "--- Testing 'exit' ---"
run_test "exit" "exit with no arguments"
run_test "exit 42" "exit with status 42"
run_test "exit 300" "exit with status 300 -> 44"
run_test "exit -1" "exit with status -1 -> 255"
run_test "exit not_a_number" "exit with non-numeric argument"
run_test "exit 1 2 3" "exit with too many arguments"

echo ""
echo "--- Testing Command Not Found ---"
run_test "nonexistentcommand" "simple command not found"
run_test "   nonexistentcommand" "command not found with leading spaces"

# --- SUMMARY ---
echo ""
echo "--- Test Summary ---"
if [ "$passed_tests" -eq "$total_tests" ]; then
    echo -e "${GREEN}All $total_tests tests passed! 🎉${NC}"
else
    echo -e "${YELLOW}$passed_tests / $total_tests tests passed.${NC}"
fi