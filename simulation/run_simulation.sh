#!/usr/bin/env bash
# Builds the real firmware sketches against a software model of the Arduino timing functions,
# records the transmitter waveform, and replays it into the receiver with clock mismatch and
# sensor delays. Needs a C++ compiler (g++ or clang++). Usage: ./run_simulation.sh
set -e
cd "$(dirname "$0")"
CXX=${CXX:-g++}
TX=../firmware/lifi_tx/lifi_tx.ino
RX=../firmware/lifi_rx/lifi_rx.ino
B=build
mkdir -p $B

build() {  # $1 = bit period in microseconds
  sed -E "s/(BIT_US +)= [0-9]+;/\1= $1;/" $TX > $B/tx.cpp
  sed -E "s/(BIT_US +)= [0-9]+;/\1= $1;/" $RX > $B/rx.cpp
  $CXX -O1 -include stub.h -x c++ $B/tx.cpp tx_main.cpp -o $B/tx_sim
  $CXX -O1 -include stub.h -x c++ $B/rx.cpp rx_main.cpp -o $B/rx_sim
}

scenario() {  # label bit_us seconds clock_scale start_offset_us rise_us fall_us
  build "$2"
  (cd $B && ./tx_sim "$3")
  out=$(cd $B && ./rx_sim "$4" "$5" "$6" "$7" "$3" || true)
  decoded=$(echo "$out" | grep "^Received:" | sed 's/Received: //' | tr -d '\n')
  printf "%-62s -> %s\n" "$1" "$decoded"
}

echo "Decoded text for each scenario (the frame in progress at start-up is skipped by design):"
scenario "10 ms/bit (100 bps), clock +0.5%"                         10000   8 1.005 333333 0 0
scenario "10 ms/bit (100 bps), clock -0.5%"                         10000   8 0.995 333333 0 0
scenario "2 ms/bit (500 bps), clock -0.5%"                           2000   5 0.995  77777 0 0
scenario "416 us/bit (about 2.4 kbps), clock +0.5%"                   416   3 1.005  12345 0 0
scenario "200 ms/bit, LDR-like delays, clock -0.5%, start 1.5 s"   200000  70 0.995 1500000 20000 40000
scenario "200 ms/bit, LDR-like delays, clock -0.5%, start 9 s"     200000  70 0.995 9000000 20000 40000
