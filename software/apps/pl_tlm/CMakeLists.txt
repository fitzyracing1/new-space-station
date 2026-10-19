# SPDX-License-Identifier: MIT
project(PINGLITE_TLM C)
get_filename_component(PL_APPS_DIR "${CMAKE_CURRENT_LIST_DIR}/.." REALPATH)
add_cfe_app(pl_tlm fsw/src/pl_tlm_app.c fsw/src/pl_tlm_logic.c)
target_include_directories(pl_tlm PUBLIC fsw/src ${PL_APPS_DIR}/pl_inc)
