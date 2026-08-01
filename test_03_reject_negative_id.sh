#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M2 Test 03 — Negative or zero ID is rejected with an error
#
# What to implement:
#   In parse_command(), after reading the id token, convert it to
#   a signed integer and check > 0. If not, return
#   ParseResult::SYNTAX_ERROR and set error = "ID must be positive."
#   (or similar). The executor prints the error. "Spawned." must
#   NOT appear.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M2/03 — Negative and zero IDs rejected"
OK=true
W=$(new_world)

OUT_NEG=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER -1 alice a@a.gg")
OUT_ZERO=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 0 alice a@a.gg")

assert_contains "$OUT_NEG" "error\|invalid\|positive\|must" \
    "SPAWN PLAYER -1 must produce an error" || OK=false
assert_not_contains "$OUT_NEG" "spawned" \
    "SPAWN PLAYER -1 must NOT say 'Spawned'" || OK=false

assert_contains "$OUT_ZERO" "error\|invalid\|positive\|must" \
    "SPAWN PLAYER 0 must produce an error" || OK=false
assert_not_contains "$OUT_ZERO" "spawned" \
    "SPAWN PLAYER 0 must NOT say 'Spawned'" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
