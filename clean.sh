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

# Check the arguments.
# Accept "tests", "build", or no arguments

if [ $# -gt 0 ]; then
    if [ "$1" != "tests" ] && [ "$1" != "build" ]; then
        echo "Usage: $0 [tests|build]"
        exit 1
    fi
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
pushd $SCRIPT_DIR > /dev/null

if [ $# -eq 0 ] || [ "$1" == "build" ]; then
    # Remove libbkmalloc.so and libheappulse.so
    rm -f libbkmalloc.so libheappulse.so
    echo "Removed libbkmalloc.so and libheappulse.so✅"

    # Remove the build directory
    rm -rf build
    echo "Removed build directory✅"
    if [ $# -eq 1 ]; then
        echo "Cleaned up build🧹✨"
        exit 0
    fi
fi

if [ $# -eq 0 ] || [ "$1" == "tests" ]; then
    # Remove the tests/*/test.exe files
    rm -f tests/*/test.exe
    echo "Removed tests/*/test.exe files✅"

    # Remove the tests/*/results directories
    rm -rf tests/*/results
    echo "Removed tests/*/results directories✅"
    if [ $# -eq 1 ]; then
        echo "Cleaned up tests🧹✨"
        exit 0
    fi
fi

echo "Cleaned up everything🧹✨"
popd > /dev/null
