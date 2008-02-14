project(plasma-am3rok)
 
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
find_package(Plasma REQUIRED)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
set(am3rokclient_SRCS amarok_client.cpp
                      mmbuttonwidget.cpp
                      mminfodisplay.cpp
                      mminfodisplaywidgetqt.cpp
                      mminfodisplaywidgetplasma.cpp
                      mminfodialog.cpp)

kde4_add_plugin(plasma_applet_am3rok ${am3rokclient_SRCS})
target_link_libraries(plasma_applet_am3rok
                      ${PLASMA_LIBS} ${KDE4_KDEUI_LIBS})
install(TARGETS plasma_applet_am3rok
        DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-am3rok.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
