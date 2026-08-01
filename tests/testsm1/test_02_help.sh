#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M1 Test 02 — .help lists all meta-commands
#
# What to implement:
#   handle_meta(".help") must print a usage summary that mentions
#   at least ".quit" and ".help". Other commands (.version, .status)
#   are strongly recommended. Returns MetaResult::SUCCESS.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/../../helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M1/02 — .help lists commands"
OK=true

OUT=$(run_no_world "$BINARY" ".help")

assert_contains "$OUT" "quit"    "Help must mention .quit"   || OK=false
assert_contains "$OUT" "help"    "Help must mention .help"   || OK=false
assert_contains "$OUT" "version" "Help must mention .version (recommended)" || true  # soft

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
