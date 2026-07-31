#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M1 Test 04 — Non-dot input goes to the world command handler
#
# What to implement:
#   The REPL dispatches lines starting with '.' to the meta handler
#   and all other lines to the world command handler.
#   For M1, the world handler is a stub — it prints something like
#   "[World command queued: <input>]" and does not crash.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/../helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M1/04 — Non-dot lines dispatched to world handler"
OK=true

OUT=$(run_no_world "$BINARY" "SPAWN PLAYER 1 alice a@a.gg" "LIST PLAYERS")

# Binary must not crash
if [[ -z "$OUT" ]]; then
    echo "  FAIL: No output produced (crash?)"
    OK=false
fi

# Must NOT misidentify a world command as a meta-command
assert_not_contains "$OUT" "unknown command.*SPAWN" \
    "SPAWN should not trigger the 'unknown dot-command' path" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
