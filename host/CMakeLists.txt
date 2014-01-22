    cmake_minimum_required(VERSION 2.8)

FIND_PACKAGE(Qt4 REQUIRED)
#Include repertories
INCLUDE_DIRECTORIES(
    includes
	${QT_INCLUDES}
	/usr/include/qwt/
	gui/Interface_Logger
 lib/pugixml/src
)

# not working when includes file in an other directory
#set(CMAKE_AUTOMOC TRUE)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(lib/)

#List source files
FILE( GLOB source_files_Logger ./*.cpp  ./src/*.cpp )
FILE( GLOB mocable_source_files_Logger ./includes/Logger.h )

FILE( GLOB IHM_SOURCES gui/Interface_Logger/*.cpp )
FILE( GLOB IHM_HEADERS gui/Interface_Logger/*.h)

FILE( GLOB SOURCES_UI_SOURCES gui/Interface_Logger/*.ui)


QT4_WRAP_UI(SOURCES_UI_HEADERS gui/Interface_Logger/IHM_Logger.ui)
QT4_WRAP_UI(SOURCES_UI_HEADERS ${SOURCES_UI_SOURCES}   )
QT4_WRAP_CPP(IHM_HEADERS_MOC ${IHM_HEADERS} ${mocable_source_files_Logger} )

MESSAGE ( Source ui : ${SOURCES_UI_HEADERS} , IHM headers moc : ${IHM_HEADERS_MOC} )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

#Ajout de l'executable du Noyau
add_executable( logger ${source_files_Logger} ${IHM_SOURCES} ${IHM_HEADERS} ${SOURCES_UI_HEADERS} ${IHM_HEADERS_MOC}  )

target_link_libraries (logger ${QT_LIBRARIES} QtSerialPort pugixml qwt )
