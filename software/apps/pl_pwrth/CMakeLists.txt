# SPDX-License-Identifier: MIT
project(PINGLITE_PWRTH C)
get_filename_component(PL_APPS_DIR "${CMAKE_CURRENT_LIST_DIR}/.." REALPATH)
add_cfe_app(pl_pwrth fsw/src/pl_pwrth_app.c fsw/src/pl_pwrth_logic.c)
target_include_directories(pl_pwrth PUBLIC fsw/src ${PL_APPS_DIR}/pl_inc)
