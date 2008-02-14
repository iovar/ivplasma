project(plasma-netgraph)
 
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(Plasma REQUIRED)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
set(netgraph_SRCS netgraph.cpp
                  blockgraph.cpp 
                  netgraph_config.cpp)

kde4_add_ui_files(netgraph_SRCS conf.ui )
kde4_add_plugin(plasma_applet_netgraph ${netgraph_SRCS})
target_link_libraries(plasma_applet_netgraph
                      ${PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
install(TARGETS plasma_applet_netgraph
        DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-netgraph.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
