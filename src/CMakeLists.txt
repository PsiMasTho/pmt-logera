file(GLOB LIB_SOURCES "*.cc")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cc")

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Make an automatic library - will be static or dynamic based on user setting
add_library(${PROJECT_NAME} ${LIB_SOURCES} ${PROJ_PUBLIC_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>)

target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include/detail>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}/detail>)

# All users of this library will need at least C++20
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-config
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}-config
        FILE ${PROJECT_NAME}-config.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

option(BUILD_LOGERA_TO_CSV "Build logera to csv program" OFF)

if (BUILD_LOGERA_TO_CSV)
    add_executable(${PROJECT_NAME}-to-csv main.cc)
    target_link_libraries(${PROJECT_NAME}-to-csv ${PROJECT_NAME})
endif ()