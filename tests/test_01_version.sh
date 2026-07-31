#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M1 Test 01 — .version prints engine name and version
#
# What to implement:
#   handle_meta(".version") should print a string that contains
#   "Chronicle" and a version number (e.g. "v0.1").
#   It should return MetaResult::SUCCESS (not QUIT or UNKNOWN).
# ─────────────────────────────────────────────────────────────────
source "../helpers.sh"
BINARY="${1:-../chronicle}"
NAME="M1/01 — .version output"
OK=true

OUT=$(run_no_world "$BINARY" ".version")

assert_contains "$OUT" "chronicle"    "Output must contain 'Chronicle'" || OK=false
assert_contains "$OUT" "v[0-9]"       "Output must contain a version like v0.1" || OK=false
assert_not_contains "$OUT" "unknown"  "'Unknown command' should NOT appear" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
