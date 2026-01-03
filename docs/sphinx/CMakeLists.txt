find_package(Python3 COMPONENTS Interpreter REQUIRED)

addcustomtarget(sphinx_docs
    COMMAND ${Python3_EXECUTABLE} -m sphinx -b html
            ${CMAKECURRENTSOURCE_DIR}
            ${CMAKECURRENTBINARYDIR}/build/html
    WORKINGDIRECTORY ${CMAKECURRENTSOURCEDIR}
    COMMENT "Generating Atoqu Sphinx documentation"
)

adddependencies(docs sphinxdocs)
