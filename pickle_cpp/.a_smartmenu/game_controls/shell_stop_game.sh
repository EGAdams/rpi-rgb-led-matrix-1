#!/bin/bash

# Run the 'ps -ef' command and filter for 'run'
ps -ef | grep run

# Extract the PID of the './run_remote_listener' process
PID=$(ps -ef | grep './run_remote_listener' | grep -v grep | awk '{print $2}')

# Check if PID exists and kill the process
if [[ -n "$PID" ]]; then
    echo "Killing process ./run_remote_listener with PID $PID"
    kill -9 "$PID"
else
    echo "No ./run_remote_listener process found"
fi

sudo pkill -9 -f "./run_remote_listener"

