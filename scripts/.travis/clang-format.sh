#!/bin/bash
set -e

if [[ -n $(git ls-files --modified) ]]; then
	echo "[ERROR] Invalid state"
	exit 1
fi

pushd scripts

clang-format --version
./run-clang-format.sh

popd # scripts

CLANG_FORMAT_DIFF=$(git ls-files --modified)
if [[ -n $CLANG_FORMAT_DIFF ]]; then
    echo "#################################################"
    echo "[WARNING] Unformatted files detected:"
    echo $CLANG_FORMAT_DIFF
    echo "#################################################"
    echo ""
    git diff
    #exit 1
else
	echo "clang-format rules: 👍"
fi

exit 0
