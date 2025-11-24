savedcmd_uaf.mod := printf '%s\n'   uaf.o | awk '!x[$$0]++ { print("./"$$0) }' > uaf.mod
