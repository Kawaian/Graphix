########################################################
################### DO NOT EDIT ME ! ##################
########################################################

SET (   ASSIMP_SEARCH_DIRS
        ${ASSIMP_ROOT_DIR}
        ./external/Assimp
        $ENV{PROGRAMFILES}/ASSIMP         # FOR WINDOWS
#       "$ENV{PROGRAMFILES(x86)}/ASSIMP"  # FOR WINDOWS
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

FIND_PATH (   ASSIMP_INCLUDE_DIRS
              NAMES
                  assimp/mesh.h
              PATHS
                  ${ASSIMP_SEARCH_DIRS}
              PATH_SUFFIXES
                  include
              DOC
                  "Assimp/mesh.h Include Path"
          )

###################### FIND LIBRARY #######################

FIND_LIBRARY (  ASSIMP_LIBRARIES
                NAMES
                  assimp ASSIMP
                PATHS
                  ${ASSIMP_SEARCH_DIRS}
                PATH_SUFFIXES
                  lib
                  lib64
                  lib/x86
                  lib/x64
                DOC
                    "Assimp Lib Path"
            )

#Did we find it ?

IF (ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES)
      SET(ASSIMP_FOUND TRUE)
      MESSAGE(STATUS "Looking for Assimp - found")
ELSE (ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES)
      SET(ASSIMP_FOUND FALSE)
      MESSAGE(STATUS "Looking for Assimp - not found")
ENDIF (ASSIMP_INCLUDE_DIRS AND ASSIMP_LIBRARIES)
