
!/usr/bin/env bash
set -e
cmake -S ../.. -B ../../build -DCMAKEBUILDTYPE=Release
cmake --build ../../build
