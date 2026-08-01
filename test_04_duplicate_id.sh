#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M2 Test 04 — Duplicate primary key is rejected
#
# What to implement:
#   WorldState::spawn() must scan existing entities to check if the
#   same id already exists. If found, return SpawnResult::DUPLICATE.
#   The executor prints an error like "Error: Entity already exists."
#   The count must remain 1 after the failed second spawn.
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M2/04 — Duplicate ID rejected"
OK=true
W=$(new_world)

OUT=$(run_cmd "$BINARY" "$W" \
    "SPAWN PLAYER 1 alice a@a.gg" \
    "SPAWN PLAYER 1 bob   b@b.gg" \
    "LIST PLAYERS")

assert_contains "$OUT" "error\|duplicate\|exists\|already" \
    "Second spawn with same id must produce an error" || OK=false

assert_not_contains "$OUT" "Spawned.*Spawned\|spawned.*spawned" \
    "Must not say 'Spawned' twice" || OK=false

# Only alice should be in the world, not bob (who was rejected)
assert_contains "$OUT" "alice" "alice (first spawn) must be in listing" || OK=false

# Count should be 1
assert_contains "$OUT" "1 entit\|1\b" "Count should be 1 after duplicate rejection" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
