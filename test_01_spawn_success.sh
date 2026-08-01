#!/usr/bin/env bash
# ─────────────────────────────────────────────────────────────────
# M2 Test 01 — SPAWN PLAYER succeeds and confirms the action
#
# What to implement:
#   parse_command("SPAWN PLAYER 1 alice alice@ironhold.gg", ...) must
#   return ParseResult::SUCCESS with CommandType::SPAWN, id=1,
#   username="alice", email="alice@ironhold.gg".
#   execute() then calls world.spawn() which returns SpawnResult::SUCCESS,
#   and the executor prints "Spawned." (or similar success message).
# ─────────────────────────────────────────────────────────────────
source "$(dirname "$0")/helpers.sh"
BINARY="${1:-./chronicle}"
NAME="M2/01 — SPAWN PLAYER prints success"
OK=true
W=$(new_world)

OUT=$(run_cmd "$BINARY" "$W" "SPAWN PLAYER 1 alice alice@ironhold.gg")

assert_contains "$OUT" "spawn\|success\|ok\|creat" \
    "Spawn must print a success/confirmation message" || OK=false

assert_not_contains "$OUT" "error" \
    "No error should appear on valid SPAWN" || OK=false

$OK && pass "$NAME" || { echo "FAIL: $NAME"; exit 1; }
