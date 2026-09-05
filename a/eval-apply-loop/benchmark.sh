#!/usr/bin/env bash

set -u

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
INPUT="$SCRIPT_DIR/fib.scm"
RUNS=5
TIMEFORMAT='%R %U %S'

for evaluator in myeval.scm myeval-namedlet.scm; do
    printf '\n== %s ==\n' "$evaluator"
    printf 'run real(s) user(s) sys(s)\n'

    for run in $(seq 1 "$RUNS"); do
        printf '%d ' "$run"
        { time gosh -l "$SCRIPT_DIR/$evaluator" -e '(driver-loop)' \
            < "$INPUT" > /dev/null; } 2>&1
    done
done
