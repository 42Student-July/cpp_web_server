FROM debian:buster


RUN apt -y update && apt -y upgrade && apt -y install nginx  vim fcgiwrap

COPY ./www /var/www
COPY ./conf/nginx/default.conf /etc/nginx/conf.d/default.conf
COPY ./conf/nginx/entrypoint.sh /
RUN chmod +x /entrypoint.sh
RUN chmod +x -R /var/www/cgi-bin/*.cgi
ENTRYPOINT ["/entrypoint.sh"]
CMD ["nginx", "-g" , "daemon off;"]
