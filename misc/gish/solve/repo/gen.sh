#!/bin/sh
mkdir execs
cp $(git --exec-path)/git ./execs/
# pick some git file which is written in bash (not a compiled binary)
cat code.sh > execs/git-mergetools
