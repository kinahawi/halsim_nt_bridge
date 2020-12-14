project(halsim_ds_bridge)

include(CompileWarnings)

file(GLOB halsim_ds_bridge_src src/main/native/cpp/*.cpp)

add_library(halsim_ds_bridge SHARED ${halsim_ds_socket_src})
wpilib_target_warnings(halsim_ds_bridge)
set_target_properties(halsim_ds_bridge PROPERTIES DEBUG_POSTFIX "d")
target_link_libraries(halsim_ds_bridge PUBLIC hal ntcore)

target_include_directories(halsim_ds_bridge PRIVATE src/main/native/include)

set_property(TARGET halsim_ds_bridge PROPERTY FOLDER "libraries")

install(TARGETS halsim_ds_bridge EXPORT halsim_ds_bridge DESTINATION "${main_lib_dest}")
