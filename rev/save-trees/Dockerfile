FROM python:3.10-slim-buster AS app
RUN pip install --no-cache-dir pycryptodome

FROM pwn.red/jail:0.3.0

COPY --from=app / /srv
COPY server.py /srv/app/run
COPY flag.txt /srv/app/

ENV JAIL_MEM=20M
