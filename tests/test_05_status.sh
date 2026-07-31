#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M1 Test 05 — .status and empty lines do not crash
#
# What to implement:
#   .status returns MetaResult::SUCCESS and prints something about
#   the world state (at minimum "World: offline" for M1).
#   Empty lines must be silently skipped — the prompt should
#   reappear without error output.
# ─────────────────────────────────────────────────────────────────
source "../helpers.sh"
BINARY="${1:-../chronicle}"
NAME="M1/05 — .status responds; empty lines skipped"
OK=true

# Feed: empty line, empty line, .status, empty line, .version
OUT=$(printf '\n\n.status\n\n.version\n.quit\n' | "$BINARY" 2>&1)

assert_contains "$OUT" "world\|World" \
    ".status must mention 'World'" || OK=false

assert_contains "$OUT" "chronicle\|Chronicle" \
    ".version must work after empty lines" || OK=false

# No error output for empty lines
assert_not_contains "$OUT" "unknown.*empty\|error.*empty" \
    "Empty lines should produce no error" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
