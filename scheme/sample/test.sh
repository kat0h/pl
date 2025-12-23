#!/bin/bash

# Ensure the interpreter 'scm' is built
echo "Building the 'scm' interpreter..."
# Navigate to the parent directory to run make, then return
if ! make; then
    echo "Error: 'scm' interpreter build failed. Aborting tests."
    exit 1
fi

echo "Starting regression tests for sample Scheme programs..."

# Iterate over all .scm files in the current directory (sample/)
for test_file in sample/*.scm; do # Corrected glob pattern
    echo "RUNNING TEST: $test_file"

    # Get the content of the Scheme file to pass as an argument
    # The 'scm' executable is in the project root.
    output=$(./scm "$(cat "$test_file")" 2>&1) # Corrected executable path
    exit_code=$?

    if [ $exit_code -ne 0 ]; then
        echo "FAILED TEST: $test_file"
        echo "-------------------- Error Output --------------------"
        echo "$output"
        echo "------------------------------------------------------"
        exit 1 # Abnormally terminate the script
    fi
    echo "PASSED TEST: $test_file"
done

echo "All regression tests passed successfully!"
exit 0