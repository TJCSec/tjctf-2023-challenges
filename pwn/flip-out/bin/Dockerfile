FROM pwn.red/jail:0.3.0

COPY --from=ubuntu:jammy-20220815 / /srv

WORKDIR /srv/app/

COPY ./flag.txt .
COPY ./chall run
