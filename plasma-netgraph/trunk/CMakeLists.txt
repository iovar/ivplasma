project(plasma-netgraph)
 
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(Plasma REQUIRED)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(custom_include_DIRS applet/
                        engine/)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ${custom_include_DIRS}
   )

set(netgraph_SRCS applet/netgraph.cpp
                  applet/blockgraph.cpp 
                  applet/netgraph_config.cpp)

kde4_add_ui_files(netgraph_SRCS applet/conf.ui )
kde4_add_plugin(plasma_applet_netgraph ${netgraph_SRCS})
target_link_libraries(plasma_applet_netgraph
                      ${PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_netgraph
        DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES applet/plasma-applet-netgraph.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})



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
    engine/netspeed_engine.cpp
    engine/knetproc.cpp
)

kde4_add_plugin(plasma_engine_networkspeed ${networkspeed_engine_SRCS})

target_link_libraries(plasma_engine_networkspeed ${KDE4_KDECORE_LIBS} plasma)

install(TARGETS plasma_engine_networkspeed DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES engine/plasma-dataengine-networkspeed.desktop DESTINATION ${SERVICES_INSTALL_DIR} )



