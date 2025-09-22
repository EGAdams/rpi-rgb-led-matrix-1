import subprocess
import os
import signal

def kill_processes_by_name(name):
    try:
        # Get all processes
        result = subprocess.run(['ps', '-ef'], capture_output=True, text=True)
        lines = result.stdout.splitlines()

        for line in lines:
            if name in line and 'grep' not in line:
                parts = line.split()
                pid = int(parts[1])
                print(f"Killing {name} process with PID: {pid}")
                os.kill(pid, signal.SIGKILL)

    except Exception as e:
        print(f"Error occurred: {e}")

if __name__ == "__main__":
    kill_processes_by_name( "run_pickle_listener" )
    kill_processes_by_name( "pickle_cpp_remote"   )
    kill_processes_by_name( "tennis-game"         )
