#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M2 Test 02 — LIST PLAYERS shows all spawned entities with count
#
# What to implement:
#   execute() on CommandType::LIST calls world.list_all(cb),
#   which invokes cb for each stored PlayerRecord.
#   Output each entity as "[id] username <email>" and print a count
#   line such as "2 entities." at the end.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M2/02 — LIST PLAYERS shows entities and count"
OK=true
W=$(new_world)

OUT=$(run_cmd "$BINARY" "$W" \
    "SPAWN PLAYER 1 alice alice@ironhold.gg" \
    "SPAWN PLAYER 2 bob   bob@ironhold.gg"   \
    "LIST PLAYERS")

assert_contains "$OUT" "alice"    "alice must appear in listing"         || OK=false
assert_contains "$OUT" "bob"      "bob must appear in listing"           || OK=false
assert_contains "$OUT" "2"        "Count of 2 must appear"               || OK=false

# Entity lines should have the [ID] format
assert_contains "$OUT" "\[1\]\|\[2\]" "Entity IDs should appear in [N] format" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
