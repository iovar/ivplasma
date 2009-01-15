project(toggle_compositing)
 
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )

set(toggle_compositing_SRCS toggle_compositing.cpp
                            confirmation_dialog.cpp
                            ktimerdialog.cpp)

kde4_add_plugin(plasma_applet_toggle_compositing ${toggle_compositing_SRCS})

target_link_libraries(plasma_applet_toggle_compositing 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS})

install(TARGETS plasma_applet_toggle_compositing
        DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-toggle_compositing.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES images/onoff_switch.svg
        DESTINATION ${DATA_INSTALL_DIR}/desktoptheme/default/widgets/)
