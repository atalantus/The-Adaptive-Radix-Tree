add_subdirectory(art)
add_subdirectory(art_virt)
add_subdirectory(art_crtp)
add_subdirectory(trie)
add_subdirectory(mtrie)
add_subdirectory(htrie)
add_subdirectory(sorted_list)
add_subdirectory(hash_table)
add_subdirectory(rbtree)

add_library(data_structures INTERFACE)

target_link_libraries(data_structures INTERFACE art art_virt art_crtp trie mtrie htrie sorted_list hash_table rbtree)
