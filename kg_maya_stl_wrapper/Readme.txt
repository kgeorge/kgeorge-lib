The solution files for both VS2003, and VC2008 express
are found in the subdirectories of the 'bld' directory.
The test program tests all aspects of the MayaSDK container
arrays from an std::vector interface


The runtime needs Mfc80.dll, which is included in the bin directory.

Incremental linking is disabled in all projects due to the
fact that both the VS2003and the VC2008_express project files,
have the same $(TargetDir) where the incremental link file appears.

