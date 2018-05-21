aux_source_directory(. DIR_DFD_SRC)

add_library(dfd ${DIR_DFD_SRC})

target_link_libraries(dfd table)