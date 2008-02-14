project(plasma-networkspeed)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(Plasma REQUIRED)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set(networkspeed_engine_SRCS
    netspeed_engine.cpp
    knetproc.cpp
)

kde4_add_plugin(plasma_engine_networkspeed ${networkspeed_engine_SRCS})
target_link_libraries(plasma_engine_networkspeed ${KDE4_KDECORE_LIBS} plasma)

install(TARGETS plasma_engine_networkspeed DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-dataengine-networkspeed.desktop DESTINATION ${SERVICES_INSTALL_DIR} )

