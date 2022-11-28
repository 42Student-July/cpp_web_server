FROM debian:buster


RUN apt-get -y update && apt-get -y install nginx

COPY ./www/*.html /var/www/html/
COPY ./conf/nginx/default.conf /etc/nginx/conf.d/default.conf

CMD ["nginx", "-g" , "daemon off;"]
