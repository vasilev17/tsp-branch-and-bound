#!/bin/bash

# Define nodes
NODES=($(oarprint host | tr ' ' '\n' | cut -d. -f1 | sort -u))
TASK_NAME=$1

# Paths and filenames
CONCORDE_PATH="/home/$USER/public/concorde/TSP/concorde"
INPUT_FILE="/home/$USER/public/concorde/${TASK_NAME}"
SHARED_DIR="/home/$USER/public/concorde"
DONE_FILE="$SHARED_DIR/.done_flag"

# Clean any previous run
rm -f "$DONE_FILE" "$SHARED_DIR"/output_*.sol
for NODE in "${NODES[@]}"; do
  rm -rf "$SHARED_DIR/run_$NODE"
done

# Launch jobs in parallel on each node with different seeds
for NODE in "${NODES[@]}"; do
  ssh "$USER@$NODE.grenoble.grid5000.fr" bash -c "'
    WORKDIR=\"$SHARED_DIR/run_$NODE\"
    mkdir -p \"\$WORKDIR\"
    cp \"$INPUT_FILE\" \"\$WORKDIR/\"
    cd \"\$WORKDIR\"

    if [ ! -f \"$DONE_FILE\" ]; then
      SEED=$(od -An -N4 -tu4 < /dev/urandom | tr -d ' ')
      \"$CONCORDE_PATH\" -s \$SEED \"$(basename "$INPUT_FILE")\" > \"output_${NODE}.sol\" 2>&1
      cp \"output_${NODE}.sol\" \"$SHARED_DIR/\"
      touch \"$DONE_FILE\"
    fi
  '" &
done

# Wait for all background jobs to finish
wait

# Print total running time for each node
for NODE in "${NODES[@]}"; do
  RESULT_FILE="$SHARED_DIR/output_${NODE}.sol"
  if [ -f "$RESULT_FILE" ]; then
    RUNNING_TIME=$(grep "Total Running Time" "$RESULT_FILE" | awk '{print $4}')
    echo "Node $NODE - Total Running Time: $RUNNING_TIME seconds"
  else
    echo "Node $NODE did not produce a result."
  fi
done