#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M1 Test 03 — Unknown dot-commands produce an error, not a crash
#
# What to implement:
#   handle_meta(".xyzzy") must return MetaResult::UNKNOWN and print
#   some kind of error or hint (e.g. "Unknown command. Try .help").
#   The process must NOT exit early — the REPL must continue until
#   .quit is sent.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/../helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M1/03 — Unknown dot-command handled gracefully"
OK=true

# Send unknown command, then .version to confirm REPL is still alive
OUT=$(run_no_world "$BINARY" ".xyzzy" ".version")

# Binary must not crash (we'd get no output if it did)
assert_contains "$OUT" "chronicle\|v0\|Unknown\|unknown\|error\|Try" \
    "Must produce some output after unknown command" || OK=false

# .version (sent after .xyzzy) must still work → REPL kept running
assert_contains "$OUT" "chronicle" \
    ".version must still work after unknown command (REPL must keep running)" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
