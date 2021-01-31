project(caller)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( caller caller caller.cpp )
target_include_directories( caller PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( caller ${CMAKE_SOURCE_DIR}/../ricardian )