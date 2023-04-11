FROM alpine:latest AS builder

WORKDIR /tmp

COPY src/* ./src/
COPY index.html .
COPY toolchain_case_compile.sh .
# RUN ./toolchain_case_compile.sh &> /dev/null
RUN ./toolchain_case_compile.sh

FROM nginx:alpine

COPY --from=builder /tmp/ /usr/share/nginx/html

RUN rm /etc/nginx/conf.d/default.conf
COPY nginx.conf /etc/nginx/nginx.conf

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
