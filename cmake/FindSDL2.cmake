########################################################
################### DO NOT EDIT ME ! ##################
########################################################

SET (   SDL2_SEARCH_DIRS
        ${SDL2_ROOT_DIR}
        ./external/SDL2
        $ENV{PROGRAMFILES}/SDL2         # FOR WINDOWS
#       "$ENV{PROGRAMFILES(x86)}/SDL2"  # FOR WINDOWS
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

FIND_PATH (   SDL2_INCLUDE_DIRS
              NAMES
              SDL.h SDL2/SDL.h
              PATHS
                  ${SDL2_SEARCH_DIRS}
              PATH_SUFFIXES
                  include
              DOC
                  "SDL2 Include Path"
          )

###################### FIND LIBRARY #######################

FIND_LIBRARY (  SDL2_LIBRARIES
                NAMES
                  SDL2 SDL2main
                PATHS
                  ${SDL2_SEARCH_DIRS}
                PATH_SUFFIXES
                  lib
                  lib64
                  lib/x86
                  lib/x64
                DOC
                    "SDL2 Lib Path"
            )

#Did we find it ?

IF (SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES)
      SET(SDL2_FOUND TRUE)
      MESSAGE(STATUS "Looking for SDL2 - found")
ELSE (SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES)
      SET(SDL2_FOUND FALSE)
      MESSAGE(STATUS "Looking for SDL2 - not found")
ENDIF (SDL2_INCLUDE_DIRS AND SDL2_LIBRARIES)
