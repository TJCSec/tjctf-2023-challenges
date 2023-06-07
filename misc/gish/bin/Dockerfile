FROM ubuntu:jammy-20230425

ENV TINI_VERSION v0.19.0
ADD https://github.com/krallin/tini/releases/download/${TINI_VERSION}/tini /tini
RUN chmod +x /tini
ENTRYPOINT ["/tini", "--"]

RUN apt-get update && apt-get install -y sudo git xinetd python3

COPY xinetd.conf /etc/xinetd.conf
COPY run.py flag.txt /srv/
RUN chmod a+x /srv/run.py
RUN mv /srv/flag.txt /flag-$(cat /proc/sys/kernel/random/uuid).txt

COPY git-cache-meta.sh /usr/lib/git-core/git-cache-meta
RUN chmod a+x /usr/lib/git-core/git-cache-meta

RUN echo "nobody ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers

CMD ["xinetd", "-dontfork"]
EXPOSE 5000
