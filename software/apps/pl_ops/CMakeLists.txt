# SPDX-License-Identifier: MIT
project(PINGLITE_OPS C)
get_filename_component(PL_APPS_DIR "${CMAKE_CURRENT_LIST_DIR}/.." REALPATH)
add_cfe_app(pl_ops fsw/src/pl_ops_app.c fsw/src/pl_ops_logic.c)
target_include_directories(pl_ops PUBLIC fsw/src ${PL_APPS_DIR}/pl_inc)
