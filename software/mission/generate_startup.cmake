# SPDX-License-Identifier: MIT
function (generate_cfs_startup_script CFS_INSTALL_DIR)
    set (STARTUP_FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/${CFS_INSTALL_DIR}/cfe_es_startup.scr")
    file (WRITE ${STARTUP_FILE}
        "CFE_APP, sch_lab,  SCH_LAB_AppMain,  SCH_LAB,  35, 32768, 0x0, 0;\n"
        "CFE_APP, ci_lab,   CI_LAB_AppMain,   CI_LAB,   40, 32768, 0x0, 0;\n"
        "CFE_APP, to_lab,   TO_LAB_AppMain,   TO_LAB,   45, 32768, 0x0, 0;\n"
        "CFE_APP, pl_eclss, PL_ECLSS_AppMain, PL_ECLSS, 50, 32768, 0x0, 0;\n"
        "CFE_APP, pl_pwrth, PL_PWRTH_AppMain, PL_PWRTH, 55, 32768, 0x0, 0;\n"
        "CFE_APP, pl_ops,   PL_OPS_AppMain,   PL_OPS,   55, 32768, 0x0, 0;\n"
        "CFE_APP, pl_tlm,   PL_TLM_AppMain,   PL_TLM,   60, 32768, 0x0, 0;\n"
    )
endfunction(generate_cfs_startup_script)
