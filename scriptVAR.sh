#!/bin/bash

# Path to your minishell executable - adjust this as needed
MINISHELL_PATH="valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell"

# Color definitions
GREEN="\033[0;32m"
RED="\033[0;31m"
YELLOW="\033[0;33m"
RESET="\033[0m"

echo -e "${YELLOW}Creating 500 export commands for minishell...${RESET}"

# Create a temporary file to store commands for minishell
TEMP_COMMANDS=$(mktemp)

# Add export commands to the temp file
for i in {1..500}
do
    echo "export VAR$i=\"This is variable $i\"" >> $TEMP_COMMANDS
    # Print a dot for progress
    echo -n "."
    # Print a new line every 50 dots for readability
    if (( i % 50 == 0 )); then
        echo " $i"
    fi
done

# Add commands to check the first and last variables
echo "" >> $TEMP_COMMANDS
echo "echo \"Checking VAR1: \$VAR1\"" >> $TEMP_COMMANDS
echo "echo \"Checking VAR500: \$VAR500\"" >> $TEMP_COMMANDS
echo "exit" >> $TEMP_COMMANDS

echo -e "\n\n${YELLOW}Running minishell with the export commands...${RESET}\n"

# Run minishell with the commands
cat $TEMP_COMMANDS | $MINISHELL_PATH

# Check the exit status of minishell
MINISHELL_STATUS=$?
if [ $MINISHELL_STATUS -eq 0 ]; then
    echo -e "\n${GREEN}Minishell completed successfully with exit code: $MINISHELL_STATUS${RESET}"
else
    echo -e "\n${RED}Minishell exited with error code: $MINISHELL_STATUS${RESET}"
fi

# Clean up the temporary file
rm $TEMP_COMMANDS

echo -e "\n${YELLOW}Test completed.${RESET}"