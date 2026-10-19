#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Builds NASA cFS (pinned submodule, unmodified in git) with the Ping Lite apps
# for native Linux. Works on a scratch copy so the submodule stays pristine.
set -euo pipefail
SW="$(cd "$(dirname "$0")/.." && pwd)"
WORK="${PL_CFS_WORK:-$SW/build/cfs}"
export CMAKE_POLICY_VERSION_MINIMUM=3.5   # some bundled apps declare cmake < 3.5
rm -rf "$WORK"; mkdir -p "$(dirname "$WORK")"
cp -a "$SW/cFS" "$WORK"
for a in pl_eclss pl_pwrth pl_ops pl_tlm pl_inc; do ln -s "$SW/apps/$a" "$WORK/apps/$a"; done
cp "$SW/mission/targets.cmake" "$SW/mission/generate_startup.cmake" "$WORK/sample_defs/"
cp "$SW/mission/install_custom.cmake" "$WORK/sample_defs/cpu1/install_custom.cmake"
rm -f "$WORK"/sample_defs/tables/*.c   # sample tables reference apps we do not build
cd "$WORK"
make native_std.prep PREP_OPTS_native_std="-DSIMULATION=native -DCFE_EDS_ENABLED=OFF -DMISSIONCONFIG=sample -DENABLE_UNIT_TESTS=FALSE"
make -j"$(nproc)" native_std.install
ls -l build-native_std/exe/cpu1/core-cpu1 build-native_std/exe/cpu1/cf/pl_*.so
