FROM debian:jessie

MAINTAINER Peter Corcoran <peter.corcoran@shadowacre.ltd>

ADD https://github.com/inspircd/inspircd/archive/v2.0.20.tar.gz /src/

RUN apt-get update
RUN apt-get install -y build-essential libssl-dev libssl1.0.0 openssl pkg-config gnutls-bin gnutls-dev sudo
RUN useradd -u 10000 -d /inspircd inspircd
WORKDIR /src
RUN tar -xvf v*.tar.gz
RUN mv `ls -d inspircd-*/` inspircd
WORKDIR inspircd

COPY src/commands/* src/commands/
COPY diffs/users.cpp.diff diffs/users.cpp.diff
RUN patch -p1 < diffs/users.cpp.diff

RUN ./configure --disable-interactive --enable-gnutls --enable-openssl --enable-epoll --prefix=/inspircd --config-dir=/inspircd/conf --log-dir=/inspircd/logs --data-dir=/inspircd/data --module-dir=/inspircd/modules --binary-dir=/inspircd/bin --uid 10000
RUN make
RUN make install
RUN apt-get purge -y build-essential
RUN rm -rf /src

EXPOSE 1025 6665 6666 6667 6668 65533 65534

RUN apt-get -y install ruby && gem install tiller
ADD config /etc/tiller
CMD ["/usr/local/bin/tiller" , "-v"]