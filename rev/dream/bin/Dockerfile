FROM pwn.red/jail:0.3.0
COPY --from=ubuntu:jammy-20220815 / /srv
COPY flag.txt /srv/app/
COPY chall /srv/app/run

ENV JAIL_TIME=60
