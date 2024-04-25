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

if (( $EUID != 0 )); then
    echo "Please run as root"
    exit
fi

# Cd into the directory of this script
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
pushd $SCRIPT_DIR > /dev/null

echo "Cleaning up🧹"
echo "========================================================================="
./clean.sh

echo ""
./build-allocator.sh || { echo "Failed to build allocator❌"; exit 1; }


echo ""
echo "Building tests🚧..."
echo "========================================================================="

./tests/build-tests.sh || { echo "Failed to build tests❌"; exit 1; }

echo ""
echo "Running tests🧪..."
echo "========================================================================="

# For every test directory in /tests, compile `test.cpp` and run the executable with the input file `test.in`
for test_dir in tests/*; do
    if [ -d "$test_dir" ]; then
        # Skip directories that don't have a test.cpp file
        if [ ! -f "$test_dir/test.cpp" ]; then
            echo "Skipping test in $test_dir🚨"
            continue
        fi

        echo -ne "Running test in $test_dir🚧"\\r
        rm -Rf $test_dir/results
        mkdir $test_dir/results
        ./run.sh $test_dir/test.exe < $test_dir/test.in > $test_dir/results/test.out 2> $test_dir/results/test.err || { echo "Test in $test_dir failed with non-zero exit code❌"; continue; }
        # Move all the CSV files in the current directory to the test directory
        shopt -s nullglob # This makes *.csv expand to nothing if no files match
        csv_files=(./*.csv)
        if [ ${#csv_files[@]} -gt 0 ]; then
            for file in "${csv_files[@]}"; do
                mv "$file" "$test_dir/results"
            done
        fi

        echo "Test in $test_dir done✅      "
    fi
done

echo ""
echo "All tests done🎉✨"

popd > /dev/null