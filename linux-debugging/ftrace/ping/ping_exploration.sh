#!/bin/bash

#        SYS_SOCKET        socket(2)
#        SYS_BIND          bind(2)
#        SYS_CONNECT       connect(2)
#        SYS_LISTEN        listen(2)
#        SYS_ACCEPT        accept(2)
#        SYS_GETSOCKNAME   getsockname(2)
#        SYS_GETPEERNAME   getpeername(2)
#        SYS_SOCKETPAIR    socketpair(2)
#        SYS_SEND          send(2)
#        SYS_RECV          recv(2)
#        SYS_SENDTO        sendto(2)
#        SYS_RECVFROM      recvfrom(2)
#        SYS_SHUTDOWN      shutdown(2)
#        SYS_SETSOCKOPT    setsockopt(2)
#        SYS_GETSOCKOPT    getsockopt(2)
#        SYS_SENDMSG       sendmsg(2)
#        SYS_RECVMSG       recvmsg(2)
#        SYS_ACCEPT4       accept4(2)
#        SYS_RECVMMSG      recvmmsg(2)
#        SYS_SENDMMSG      sendmmsg(2)
        #-g __x64_sys_socket \
        #-g __x64_sys_sendto \
        #-g netlink_sendmsg \

sudo trace-cmd record -p function_graph \
        -g "ieee80211_alloc_hw_nm" \
        -n "__rcu_*" \
        -n "security_*" \
        /usr/bin/ping google.com -c1 \
        && trace-cmd report | less
