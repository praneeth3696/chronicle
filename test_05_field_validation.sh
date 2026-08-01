#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M2 Test 05 — Field length limits enforced
#
# What to implement:
#   parse_command() must validate:
#     username.size() <= 32  → error if > 32
#     email.size()    <= 255 → error if > 255
#   Exactly 32-char username must be ACCEPTED (boundary case).
#   Missing fields (incomplete SPAWN) must produce SYNTAX_ERROR.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M2/05 — Field length limits and boundary cases"
OK=true
W=$(new_world)

LONG_NAME=$(python3 -c "print('a'*33)")
EXACT_NAME=$(python3 -c "print('a'*32)")
LONG_EMAIL=$(python3 -c "print('a'*256 + '@x.gg')")

# 33-char username: must reject
OUT1=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 1 $LONG_NAME x@x.gg")
assert_contains "$OUT1" "error\|too long\|length\|username" \
    "33-char username must be rejected" || OK=false
assert_not_contains "$OUT1" "spawned" "Must not say Spawned" || OK=false

# 256-char email: must reject
OUT2=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 2 alice $LONG_EMAIL")
assert_contains "$OUT2" "error\|too long\|length\|email" \
    "256-char email must be rejected" || OK=false

# Exactly 32-char username: must accept
OUT3=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 3 $EXACT_NAME x@x.gg")
assert_contains "$OUT3" "spawn\|success\|ok" \
    "Exactly 32-char username must be accepted" || OK=false
assert_not_contains "$OUT3" "error\|too long" \
    "32-char username should NOT produce an error" || OK=false

# Missing email: must reject
OUT4=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 4 alice")
assert_contains "$OUT4" "error\|syntax\|expected" \
    "Missing email must produce a syntax error" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
