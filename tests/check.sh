#!/bin/bash
set -euo pipefail

./unit_tests
./fnf check | diff check.out -
