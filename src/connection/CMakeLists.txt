set(LIBRARY_NAME connection)

set(SOURCE_FILES 
    DeviceConnection.hpp
    NetworkDeviceFinder.hpp

    DeviceConnection.cpp
    NetworkDeviceFinder.cpp
)

add_library(${LIBRARY_NAME} ${SOURCE_FILES})

target_include_directories(${LIBRARY_NAME} 
    PUBLIC ${CMAKE_SOURCE_DIR}/src
    PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${LIBRARY_NAME} 
    ${Boost_LIBRARIES} 
)
