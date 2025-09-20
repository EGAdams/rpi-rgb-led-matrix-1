#!/usr/bin/env bash
set -euo pipefail

trap 'echo; echo "Interrupted. Exiting."; exit 130' INT

BASE_COMMAND=(
  sudo ./minimal-example
  --led-rows=64
  --led-cols=64
  --led-chain=1
  --led-parallel=1
  --led-gpio-mapping=adafruit-hat
  --led-brightness=2
)

for addr_type in {0..7}; do
  for multiplex in {0..19}; do
    current_command=("${BASE_COMMAND[@]}" --led-addr-type="${addr_type}" --led-multiplexing="${multiplex}")
    command_string=$(printf '%q ' "${current_command[@]}")
    command_string=${command_string%% } # trim trailing space

    printf '\n*******************************************************************************************\n'
    printf 'Command: %s\n\n' "${command_string}"
    printf '********************************************************************************\n\n'

    read -r -p "<enter> to continue..." _
    echo
    "${current_command[@]}"
    echo
  done
 done

echo "All address and multiplex combinations have been tested."
