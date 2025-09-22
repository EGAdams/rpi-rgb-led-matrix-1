import sys
import re
from collections import deque

def parse_make_output(lines):
    error_lines = []
    warning_lines = []
    output = []

    error_pattern = re.compile(r'error:', re.IGNORECASE)
    warning_pattern = re.compile(r'warning:', re.IGNORECASE)
    file_line_pattern = re.compile(r'^(.+?):(\d+):')

    context_queue = deque(maxlen=5)
    capture_block = []

    def flush_block():
        if capture_block:
            output.append( "\n" + capture_block[ 0 ].strip())
            for line in capture_block[1:]:
                output.append("    " + line.strip())
            capture_block.clear()

    for line in lines:
        context_queue.append(line)
        if error_pattern.search(line):
            error_lines.append(line)
            # Capture previous context if it includes a source file
            for ctx_line in reversed(context_queue):
                if file_line_pattern.match(ctx_line):
                    capture_block.append(ctx_line)
                    break
            capture_block.append(line)
            flush_block()

        elif warning_pattern.search(line):
            warning_lines.append(line)
            for ctx_line in reversed(context_queue):
                if file_line_pattern.match(ctx_line):
                    capture_block.append(ctx_line)
                    break
            capture_block.append(line)
            flush_block()

    return output, len(error_lines), len(warning_lines)

def main():
    if not sys.stdin.isatty():
        lines = sys.stdin.readlines()
    else:
        print("Usage: make 2>&1 | python3 parse_make_errors.py")
        return

    parsed_output, error_count, warning_count = parse_make_output(lines)

    print("\n--- Relevant Output ---\n")
    for line in parsed_output:
        print(line)

    print("\n--- Summary ---")
    print(f"Errors: {error_count}")
    print(f"Warnings: {warning_count}")

if __name__ == "__main__":
    main()
