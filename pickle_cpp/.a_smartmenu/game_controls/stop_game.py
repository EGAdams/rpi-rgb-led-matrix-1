import subprocess
import re
import time

def find_process_id(process_name):
    """Find the process ID of the given process."""
    result = subprocess.run(["ps", "-ef"], capture_output=True, text=True)
    lines = result.stdout.splitlines()
    
    for line in lines:
        if process_name in line and "grep" not in line:
            parts = re.split(r'\s+', line)
            return parts[1]  # PID is the second column
    return None

def kill_process(pid):
    """Kill the process with the given PID."""
    subprocess.run(["kill", "-9", pid], check=True)

def restart_process(command): # this one needs help...
    """Restart the process with sudo privileges."""
    subprocess.run(["sudo"] + command, check=True)

if __name__ == "__main__":
    process_name = "./run_remote_listener"
    
    pid = find_process_id(process_name)
    if pid:
        print(f"Found process {process_name} with PID {pid}, killing it...")
        kill_process(pid)
    
    print("Game stopped.")
    time.sleep(2)  # Wait a bit before relinquishing control
