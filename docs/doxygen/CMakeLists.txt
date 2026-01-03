find_package(Doxygen REQUIRED)

set(DOXYGENIN  ${CMAKECURRENTSOURCEDIR}/Doxyfile)
set(DOXYGENOUT ${CMAKECURRENTBINARYDIR}/Doxyfile.out)

configurefile(${DOXYGENIN} ${DOXYGEN_OUT} @ONLY)

addcustomtarget(doxygen_docs
    COMMAND Doxygen::doxygen ${DOXYGEN_OUT}
    WORKINGDIRECTORY ${CMAKECURRENTSOURCEDIR}
    COMMENT "Generating Atoqu Doxygen documentation"
)

addcustomtarget(docs ALL DEPENDS doxygen_docs)
