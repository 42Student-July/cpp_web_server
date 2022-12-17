#!/bin/sh
#service nginx restart
service  fcgiwrap restart
chmod +x -R /var/www/cgi-bin/*.cgi
echo "$@"
exec "$@"
