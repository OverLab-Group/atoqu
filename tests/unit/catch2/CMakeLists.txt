addexecutable(testcatch2unit testmain.cpp)
targetlinklibraries(testcatch2unit PRIVATE atoqu_core)
addtest(NAME unitcatch2 COMMAND testcatch2unit)
