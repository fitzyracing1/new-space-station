# SPDX-License-Identifier: MIT
project(PINGLITE_ECLSS C)
get_filename_component(PL_APPS_DIR "${CMAKE_CURRENT_LIST_DIR}/.." REALPATH)
add_cfe_app(pl_eclss fsw/src/pl_eclss_app.c fsw/src/pl_eclss_logic.c)
target_include_directories(pl_eclss PUBLIC fsw/src ${PL_APPS_DIR}/pl_inc)
