#!/bin/bash
find ../ctb/src -iname *.hpp -o -iname *.cpp -o -iname *.tcc | xargs clang-format -i -style=file
find ../common/src -iname *.hpp -o -iname *.cpp -o -iname *.tcc | xargs clang-format -i -style=file
find ../editor/src -iname *.hpp -o -iname *.cpp -o -iname *.tcc | xargs clang-format -i -style=file
find ../engine/src -iname *.hpp -o -iname *.cpp -o -iname *.tcc | xargs clang-format -i -style=file
find ../parser/src -iname *.hpp -o -iname *.cpp -o -iname *.tcc | xargs clang-format -i -style=file
