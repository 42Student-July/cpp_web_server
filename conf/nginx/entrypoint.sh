#!/bin/sh
#service nginx restart
service  fcgiwrap restart
echo "$@"
exec "$@"