#!/bin/bash

# Define the repository directory
REPO_DIR="$HOME/rpi-rgb-led-matrix/tennis-game"

# Change to the repository directory
cd "$REPO_DIR" || { echo "Failed to change directory to $REPO_DIR"; exit 1; }

# Restore all modified files
echo "Restoring all modified files..."
git restore .

# Confirm completion
echo "All changes have been restored to their last committed state."
