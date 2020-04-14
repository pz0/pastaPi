find_package(Qt5 COMPONENTS Core REQUIRED)

if (BUILD_PLUGINS_GUI)
    set(PLUGIN_SOURCES ${SOURCES} ${GUI_SOURCES})
else()
    set(PLUGIN_SOURCES ${SOURCES})
endif ()

add_library(${PROJECT_NAME} MODULE ${PLUGIN_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ROOT})
target_compile_definitions(${PROJECT_NAME}
  PUBLIC SPAGHETTI_SHARED
  PRIVATE SPAGHETTI_EXPORTS ${SPAGHETTI_DEFINITIONS}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_DEFINITIONS_DEBUG}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_DEFINITIONS_RELEASE}>
  )
target_compile_options(${PROJECT_NAME}
  PRIVATE ${SPAGHETTI_FLAGS}
  PRIVATE ${SPAGHETTI_FLAGS_C}
  PRIVATE ${SPAGHETTI_FLAGS_CXX}
  PRIVATE ${SPAGHETTI_FLAGS_LINKER}
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_FLAGS_DEBUG}>
  PRIVATE $<$<CONFIG:Debug>:${SPAGHETTI_WARNINGS}>
  PRIVATE $<$<CONFIG:Release>:${SPAGHETTI_FLAGS_RELEASE}>
  )
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/plugins")

if (BUILD_PLUGINS_GUI)
  add_compile_definitions(BUILD_PLUGIN_GUI)
  target_link_libraries(${PROJECT_NAME} Spaghetti SpaghettiUI Qt5::Core ${LIB_GUI})
else()
  target_link_libraries(${PROJECT_NAME} Spaghetti Qt5::Core ${LIB_NO_GUI})
endif ()

install(TARGETS ${PROJECT_NAME}
  COMPONENT ${PROJECT_NAME}Plugin
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/spaghetti
  )