#!/usr/bin/env bash
make -C server all vendor/websocketd/websocketd
server/server &
server/vendor/websocketd/websocketd --port 8080 --staticdir www ncat 127.0.0.1 9123 &
wait
