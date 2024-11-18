set(LIBRARY_NAME rrc_init)

set(SOURCE_FILES 
    ApplicationContext.hpp
    RrcApp.hpp

    ApplicationContext.cpp
    RrcApp.cpp
)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})

target_include_directories(${LIBRARY_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${LIBRARY_NAME} 
    ${Boost_LIBRARIES}
    rrc_connection
    rrc_ps5_ctrl
)
