########################################################
################### DO NOT EDIT ME ! ##################
########################################################

SET (   GLEW_SEARCH_DIRS
        ${GLEW_ROOT_DIR}
        ./external/Glew
        $ENV{PROGRAMFILES}/GLEW         # FOR WINDOWS
#       "$ENV{PROGRAMFILES(x86)}/GLEW"  # FOR WINDOWS
        ~/Library/Frameworks            # FOR APPLE
        /Library/Frameworks             # FOR APPLE
        /usr/local                      # UNIX BASED SYSTEMS
        /usr/
        /opt
        /sw                             # Fink
        /opt/local                      # DarwinPorts
        /opt/csw                        # BlastWave
    )

###################### FIND INCLUDE #######################

FIND_PATH (   GLEW_INCLUDE_DIRS
              NAMES
                  GL/glew.h
              PATHS
                  ${GLEW_SEARCH_DIRS}
              PATH_SUFFIXES
                  include
              DOC
                  "Glew Include Path"
          )

###################### FIND LIBRARY #######################

FIND_LIBRARY (  GLEW_LIBRARIES
                NAMES
                  glew GLEW libGLEW.a glew32 glew32s
                PATHS
                  ${GLEW_SEARCH_DIRS}
                PATH_SUFFIXES
                  lib
                  lib64
                  lib/Release/Win32
                  lib/Release/x64
                DOC
                    "Glew Lib Path"
            )

#Did we find it ?

IF (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
      SET(GLEW_FOUND TRUE)
      MESSAGE(STATUS "Looking for GLEW - found")
ELSE (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
      SET(GLEW_FOUND FALSE)
      MESSAGE(STATUS "Looking for GLEW - not found")
ENDIF (GLEW_INCLUDE_DIRS AND GLEW_LIBRARIES)
