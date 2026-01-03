enable_testing()

file(GLOBRECURSE UNITTEST_SOURCES
    CONFIGURE_DEPENDS
    unit/*.cpp
)

file(GLOBRECURSE INTEGRATIONTEST_SOURCES
    CONFIGURE_DEPENDS
    integration/*.cpp
)

file(GLOBRECURSE SECURITYTEST_SOURCES
    CONFIGURE_DEPENDS
    security/*.cpp
)

file(GLOBRECURSE PERFTEST_SOURCES
    CONFIGURE_DEPENDS
    perf/*.cpp
)

addexecutable(unittests ${UNITTESTSOURCES})
targetlinklibraries(unittests PRIVATE atoqucore)
addtest(NAME unittests COMMAND unit_tests)

addexecutable(integrationtests ${INTEGRATIONTESTSOURCES})
targetlinklibraries(integrationtests PRIVATE atoqucore)
addtest(NAME integrationtests COMMAND integration_tests)

addexecutable(securitytests ${SECURITYTESTSOURCES})
targetlinklibraries(securitytests PRIVATE atoqucore)
addtest(NAME securitytests COMMAND security_tests)

addexecutable(benchatoqu ${PERFTESTSOURCES})
targetlinklibraries(benchatoqu PRIVATE atoqucore)
