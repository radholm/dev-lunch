# Change to alpine image
FROM silkeh/clang:latest AS builder

WORKDIR /tmp

COPY src/* ./src/
COPY index.html .
COPY toolchain_map_compile.sh .

RUN ./toolchain_map_compile.sh

FROM nginx:alpine

COPY --from=builder /tmp/ /usr/share/nginx/html

EXPOSE 80

CMD ["nginx", "-g", "daemon off;"]
