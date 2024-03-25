FROM nginx:alpine

RUN rm /etc/nginx/conf.d/default.conf
COPY ./nginx/html /etc/nginx/html
COPY ./nginx/nginx.conf /etc/nginx
