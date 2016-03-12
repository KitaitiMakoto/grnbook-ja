FROM ruby:2.3
MAINTAINER KITAITI Makoto

COPY groonga.list /etc/apt/sources.list.d

RUN apt-get update && \
    apt-get install -y --allow-unauthenticated groonga-keyring && \
    apt-get update && \
    { echo mysql-server mysql-server/root-password password ''; \
      echo mysql-server mysql-server/root_password_again password ''; \
    } | debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server-mroonga groonga-tokenizer-mecab php5-cli && \
    gem install eye

COPY eye.conf /etc/
COPY eye /etc/eye/

EXPOSE 80

CMD ["eye", "load", "--foreground", "/etc/eye.conf"]
