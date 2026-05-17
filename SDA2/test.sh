#!/bin/bash
cp "tests/in/test${1}.in" indexare.in
cp "tests/out/test${1}.out" indexare.ref
make clean search_index
diff indexare.ref indexare.out > indexare.diff