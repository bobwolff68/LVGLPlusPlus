#
# Note from RMW - 2/28/23
# File originally seems to have served two purposes which may not be necessary now.
# 1. Passing -m32 to the linker
# 2. Allowing the native build output to be executed via the 'upload' button after a build.
#

Import("env", "projenv")

for e in [ env, projenv ]:
    # If compiler uses `-m32`, propagate it to linker.
    # Add via script, because `-Wl,-m32` does not work.
    if "-m32" in e['CCFLAGS']:
        e.Append(LINKFLAGS = ["-m32"])

exec_name = "${BUILD_DIR}/${PROGNAME}${PROGSUFFIX}"

# Override unused "upload" to execute compiled binary
from SCons.Script import AlwaysBuild
AlwaysBuild(env.Alias("upload", exec_name, exec_name))

# Add custom target to explorer
env.AddTarget(
    name = "execute",
    dependencies = exec_name,
    actions = exec_name,
    title = "Execute",
    description = "Build and execute",
    group="General"
)

#print('=====================================')
#print(env.Dump())
