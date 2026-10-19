# SPDX-License-Identifier: MIT
# Ping Lite v0.1 mission definition, overlaid onto cFS/sample_defs by
# software/scripts/build_cfs.sh. Native Linux simulation only.
SET(MISSION_NAME "PingLite")
SET(SPACECRAFT_ID 0x50)
SET(MISSION_CPUNAMES cpu1)
SET(cpu1_PROCESSORID 1)
SET(cpu1_APPLIST ci_lab to_lab sch_lab pl_eclss pl_pwrth pl_ops pl_tlm)
SET(cpu1_SYSTEM i686-linux-gnu)
SET(FT_INSTALL_SUBDIR "host/functional-test")
