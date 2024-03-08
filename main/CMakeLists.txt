# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.

FILE(GLOB_RECURSE app_sources *.*)

idf_component_register(SRCS ${app_sources} INCLUDE_DIRS ../include REQUIRES ulp esp_wifi nvs_flash esp_netif esp_common)

#ULP
set(ulp_app_name ulp_main)
set(ulp_s_sources "../ulp/main.S")
set(ulp_exp_dep_srcs "main.c")

ulp_embed_binary(${ulp_app_name} ${ulp_s_sources} ${ulp_exp_dep_srcs})
