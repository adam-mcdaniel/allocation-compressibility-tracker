#!/bin/bash
# 
# ██╗  ██╗███████╗ █████╗ ██████╗ ██████╗ ██╗   ██╗██╗     ███████╗███████╗
# ██║  ██║██╔════╝██╔══██╗██╔══██╗██╔══██╗██║   ██║██║     ██╔════╝██╔════╝
# ███████║█████╗  ███████║██████╔╝██████╔╝██║   ██║██║     ███████╗█████╗  
# ██╔══██║██╔══╝  ██╔══██║██╔═══╝ ██╔═══╝ ██║   ██║██║     ╚════██║██╔══╝  
# ██║  ██║███████╗██║  ██║██║     ██║     ╚██████╔╝███████╗███████║███████╗
# ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝      ╚═════╝ ╚══════╝╚══════╝╚══════╝
# 
# ███╗   ███╗███████╗███╗   ███╗ ██████╗ ██████╗ ██╗   ██╗                 
# ████╗ ████║██╔════╝████╗ ████║██╔═══██╗██╔══██╗╚██╗ ██╔╝                 
# ██╔████╔██║█████╗  ██╔████╔██║██║   ██║██████╔╝ ╚████╔╝                  
# ██║╚██╔╝██║██╔══╝  ██║╚██╔╝██║██║   ██║██╔══██╗  ╚██╔╝                   
# ██║ ╚═╝ ██║███████╗██║ ╚═╝ ██║╚██████╔╝██║  ██║   ██║                    
# ╚═╝     ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═╝   ╚═╝                    
# 
# ██████╗ ██████╗  ██████╗ ███████╗██╗██╗     ███████╗██████╗              
# ██╔══██╗██╔══██╗██╔═══██╗██╔════╝██║██║     ██╔════╝██╔══██╗             
# ██████╔╝██████╔╝██║   ██║█████╗  ██║██║     █████╗  ██████╔╝             
# ██╔═══╝ ██╔══██╗██║   ██║██╔══╝  ██║██║     ██╔══╝  ██╔══██╗             
# ██║     ██║  ██║╚██████╔╝██║     ██║███████╗███████╗██║  ██║             
# ╚═╝     ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝             
# 


# cd into the build-tests.sh directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
pushd $SCRIPT_DIR > /dev/null

# Check if there are arguments. If so, build only the tests in the specified directories

if [ $# -gt 0 ]; then
    for test_dir in "$@"; do
        if [ ! -d "$test_dir" ]; then
            echo "Directory $test_dir does not exist❌"
            echo "Usage: $0 <test directory> <test directory> ..."
            exit 1
        fi
    done
fi

# For every test directory in /tests, compile `test.cpp`
for test_dir in *; do
    if [ -d "$test_dir" ]; then
        # Check if there are arguments. If so, skip the test directories that are not in the arguments
        if [ $# -gt 0 ]; then
            if [[ ! " $@ " =~ "$test_dir" ]]; then
                continue
            fi
        fi
        # Skip directories that don't have a test.cpp file
        if [ ! -f "$test_dir/test.cpp" ]; then
            echo "Skipping test in $test_dir🚨"
            continue
        fi
        echo -ne "Building test in $test_dir🚧"\\r
        g++ $test_dir/test.cpp -o $test_dir/test.exe -g -O0 -pthread || { echo "Failed to compile test in $test_dir❌"; exit 1; }
        echo "Compiled test in $test_dir✅      "
    fi
done

popd > /dev/null