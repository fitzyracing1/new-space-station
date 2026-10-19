# SPDX-License-Identifier: MIT
set(APP_INCLUDE_TABLE_LIST
    $<TARGET_PROPERTY:ci_lab,INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:to_lab,INCLUDE_DIRECTORIES>
)
target_include_directories(to_lab.table INTERFACE ${APP_INCLUDE_TABLE_LIST})
target_include_directories(sch_lab.table INTERFACE ${APP_INCLUDE_TABLE_LIST})
install(SCRIPT ${MISSION_DEFS}/generate_startup.cmake)
install(CODE "generate_cfs_startup_script(\"${TGTNAME}/${INSTALL_SUBDIR}\" ${${TGTNAME}_APPLIST})")
