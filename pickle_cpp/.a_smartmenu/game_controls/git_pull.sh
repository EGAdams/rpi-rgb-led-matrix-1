#!/bin/bash

# Define repository directory
REPO_DIR="$HOME/rpi-rgb-led-matrix/tennis-game"

# Read GitHub password from file
GH_PASS=$(cat "$HOME/georgia_home_boy")

# Change to repository directory
cd "$REPO_DIR" || { echo "Failed to change directory to $REPO_DIR"; exit 1; }

# Perform git pull using stored credentials
echo "Pulling latest changes from GitHub..."
GIT_ASKPASS_SCRIPT=$(mktemp)
chmod +x "$GIT_ASKPASS_SCRIPT"

# Create temporary script for Git credentials
cat <<EOF > "$GIT_ASKPASS_SCRIPT"
#!/bin/bash
echo "$GH_PASS"
EOF

# Execute git pull with credentials
GIT_ASKPASS="$GIT_ASKPASS_SCRIPT" git pull https://egadams@github.com/EGAdams/rpi-rgb-led-matrix.git master
 
# Cleanup temporary script
rm -f "$GIT_ASKPASS_SCRIPT"

echo "Git pull complete."

echo "changing startup script to exe... "
chmod +x "$HOME/rpi-rgb-led-matrix/tennis-game/.a_smartmenu/game_controls/startup.sh"
echo "changing git_pull script to exe... "
chmod +x "$HOME/rpi-rgb-led-matrix/tennis-game/.a_smartmenu/game_controls/git_pull.sh"
echo "changing shell_stop_game script to exe... "
chmod +x "$HOME/rpi-rgb-led-matrix/tennis-game/.a_smartmenu/game_controls/shell_stop_game.sh"
echo "changing restore_all_files script to exe... "
chmod +x "$HOME/rpi-rgb-led-matrix/tennis-game/.a_smartmenu/game_controls/restore_all_files.sh"

echo "Update complete."
