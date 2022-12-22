#!/bin/sh
#service nginx restart
service  fcgiwrap restart
chmod +x -R /var/www/cgi-bin/*.cgi
echo "$@"
# nginx -g daemon off;
# tail -f /dev/null
exec "$@"
