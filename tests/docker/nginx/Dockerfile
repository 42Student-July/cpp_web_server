FROM debian:buster


RUN apt-get -y update && apt-get -y install nginx

COPY ./www/*.html /var/www/html/

CMD ["nginx", "-g" , "daemon off;"]