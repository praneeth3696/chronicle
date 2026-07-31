#!/usr/bin/env bash
# Shared helpers for milestone test scripts.
# Source this file: source "$(dirname "$0")/../helpers.sh"

TMPDIR_ROOT=$(mktemp -d)
trap 'rm -rf "$TMPDIR_ROOT"' EXIT

# run_cmd BINARY WORLD_FILE CMD...
# Pipes all CMDs (+ .quit) to BINARY with WORLD_FILE as argv[1].
# Returns stdout+stderr.
run_cmd() {
    local binary="$1"; local world="$2"; shift 2
    local input
    printf '%s\n' "$@" | cat - <(echo ".quit") \
        | "$binary" "$world" 2>&1
}

# run_no_world BINARY CMD...
# Like run_cmd but passes no world file (M1).
run_no_world() {
    local binary="$1"; shift
    printf '%s\n' "$@" | cat - <(echo ".quit") \
        | "$binary" 2>&1
}

# new_world  → prints path to a fresh temp world file
new_world() { echo "$TMPDIR_ROOT/test_$(date +%N).world"; }

# assert_contains OUTPUT PATTERN [MESSAGE]
assert_contains() {
    local out="$1" pattern="$2" msg="${3:-output should contain '$pattern'}"
    if echo "$out" | grep -qi "$pattern"; then return 0
    else echo "  FAIL: $msg"; echo "  Got: $(echo "$out" | tr '\n' '|' | cut -c1-200)"; return 1
    fi
}

# assert_not_contains OUTPUT PATTERN [MESSAGE]
assert_not_contains() {
    local out="$1" pattern="$2" msg="${3:-output should NOT contain '$pattern'}"
    if echo "$out" | grep -qi "$pattern"; then
        echo "  FAIL: $msg"
        echo "  Got: $(echo "$out" | tr '\n' '|' | cut -c1-200)"
        return 1
    fi
    return 0
}

# assert_sorted OUTPUT  — entity lines [N] must appear in ascending id order
assert_sorted() {
    local out="$1"
    local ids
    ids=$(echo "$out" | grep -oE '\[[0-9]+\]' | tr -d '[]')
    local sorted
    sorted=$(echo "$ids" | sort -n)
    if [[ "$ids" == "$sorted" && -n "$ids" ]]; then return 0
    else echo "  FAIL: entities not in sorted order — got: $(echo $ids)"; return 1
    fi
}

# pass TEST_NAME
pass() { echo "PASS: $1"; }
