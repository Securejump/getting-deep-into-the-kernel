savedcmd_newnull.mod := printf '%s\n'   newnull.o | awk '!x[$$0]++ { print("./"$$0) }' > newnull.mod
