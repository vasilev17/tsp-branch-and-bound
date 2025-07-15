#!/bin/bash

# 1) Setup
NODES=( $(oarprint host | tr ' ' '\n' | cut -d. -f1 | sort -u) )
TASK_NAME=$1

CONCORDE_PATH="/home/$USER/public/concorde/TSP/concorde"
SHARED_DIR="/home/$USER/public/concorde"
INPUT_FILE="$SHARED_DIR/$TASK_NAME"
DONE_FILE="$SHARED_DIR/.done_flag"

# 2) Clean up old artifacts from previous runs
rm -f "$DONE_FILE" "$SHARED_DIR"/output_*.sol
for NODE in "${NODES[@]}"; do
  rm -rf "$SHARED_DIR/run_$NODE"
done

declare -A JOB_PIDS

# 3) Launch one ssh job per node
for NODE in "${NODES[@]}"; do
  INPUT_BASE=$(basename "$INPUT_FILE")

  ssh "$USER@$NODE.grenoble.grid5000.fr" \
      WORKDIR="$SHARED_DIR/run_$NODE" \
      DONE_FILE="$DONE_FILE" \
      CONCORDE_PATH="$CONCORDE_PATH" \
      INPUT_BASE="$INPUT_BASE" \
      SHARED_DIR="$SHARED_DIR" \
      NODE="$NODE" \
      bash -s <<'REMOTE_SCRIPT' &

# -- Remote script begins here --
mkdir -p "$WORKDIR"
cp "$SHARED_DIR/$INPUT_BASE" "$WORKDIR/"
cd "$WORKDIR"

if [ ! -f "$DONE_FILE" ]; then
  SEED=$(od -An -N4 -tu4 < /dev/urandom | tr -d ' ')
  "$CONCORDE_PATH" -s "$SEED" "$INPUT_BASE" > "output_${NODE}.sol" 2>&1

  if [ -f "output_${NODE}.sol" ] && [ ! -f "$DONE_FILE" ]; then
    cp "output_${NODE}.sol" "$SHARED_DIR/" && sync
    echo "$NODE" > "$DONE_FILE"
  fi
fi

REMOTE_SCRIPT

  JOB_PIDS[$!]=$NODE
done

# 4) Wait for first to finish, then kill the rest
while [ ! -f "$DONE_FILE" ]; do sleep 1; done
for PID in "${!JOB_PIDS[@]}"; do
  kill "$PID" 2>/dev/null
done

# 5) Report result
WINNER_NODE=$(< "$DONE_FILE")
RESULT_FILE="$SHARED_DIR/output_${WINNER_NODE}.sol"

if [ -f "$RESULT_FILE" ]; then
  RUNNING_TIME=$(grep "Total Running Time" "$RESULT_FILE" | awk '{print $4}')
  echo "Node $WINNER_NODE - Total Running Time: $RUNNING_TIME seconds"
else
  echo "Node $WINNER_NODE completed but no result file was found."
fi
