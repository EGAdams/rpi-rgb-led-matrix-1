#!/bin/bash

# Find the process ID (PID) of ./run_remote_listener
PID=$(ps -ef | grep './run_remote_listener' | grep -v grep | awk '{print $2}')

# Check if the process was found
if [ -n "$PID" ]; then
    echo "Killing process ./run_remote_listener with PID: $PID"
    kill -9 "$PID"
    sleep 1  # Give some time for the process to terminate
else
    echo "No running process found for ./run_remote_listener"
fi

# Restart the process with sudo
echo "Restarting ./run_remote_listener..."
sudo ./run_remote_listener
