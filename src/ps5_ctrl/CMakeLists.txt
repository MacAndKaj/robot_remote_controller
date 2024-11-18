set(LIBRARY_NAME rrc_ps5_ctrl)

set(SOURCE_FILES 
    defs.hpp
    GamepadMonitoring.hpp
    GamepadState.hpp
    Ps5DualsenseHidAgent.hpp

    GamepadMonitoring.cpp
    Ps5DualsenseHidAgent.cpp
)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})

target_include_directories(${LIBRARY_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${LIBRARY_NAME} 
    ${Boost_LIBRARIES} 
)
