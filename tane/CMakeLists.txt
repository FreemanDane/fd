aux_source_directory(. DIR_DFD_SRC)

add_library(tane ${DIR_DFD_SRC})

target_link_libraries(tane table)