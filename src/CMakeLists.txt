file(GLOBRECURSE ATOQUCORE_SOURCES
    CONFIGURE_DEPENDS
    *.cpp
)

if (ATOQUENABLECUDA)
    file(GLOBRECURSE ATOQUCUDA_SOURCES
        CONFIGURE_DEPENDS
        gpu/cuda/*.cu
    )
    addlibrary(atoqucore STATIC ${ATOQUCORESOURCES} ${ATOQUCUDASOURCES})
    settargetproperties(atoqu_core PROPERTIES
        CUDASEPARABLECOMPILATION ON
    )
else()
    addlibrary(atoqucore STATIC ${ATOQUCORESOURCES})
endif()

targetincludedirectories(atoqu_core PUBLIC
    ${CMAKECURRENTSOURCE_DIR}/../include
    ${CMAKECURRENTSOURCE_DIR}/../utils
)

add_executable(atoqu main.cpp)
targetlinklibraries(atoqu PRIVATE atoqu_core)
