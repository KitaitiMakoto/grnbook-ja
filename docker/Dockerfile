FROM ruby:2.3
MAINTAINER KITAITI Makoto

COPY groonga.list /etc/apt/sources.list.d

RUN apt-get update && \
    apt-get install -y --allow-unauthenticated groonga-keyring && \
    apt-get update && \
    { echo mysql-server mysql-server/root-password password 'root'; \
      echo mysql-server mysql-server/root_password_again password 'root'; \
      echo phpmyadmin phpmyadmin/reconfigure-webserver multiselect none; \
    } | debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server-mroonga groonga-tokenizer-mecab phpmyadmin poppler-utils && \
    gem update --system && \
    gem update && \
    gem install eye
RUN mysqld & sleep 12 && mysql -uroot -e "INSTALL PLUGIN mroonga SONAME 'ha_mroonga.so'" && mysqladmin -uroot shutdown

COPY eye.conf /etc/
COPY eye /etc/eye/
COPY 000-default.conf /etc/apache2/sites-available/
COPY pdfsearch.conf /etc/apache2/sites-available/
RUN ln -s /etc/apache2/sites-available/pdfsearch.conf /etc/apache2/sites-enabled/

COPY config.inc.php /etc/phpmyadmin/config.inc.php

EXPOSE 80

CMD ["eye", "load", "--foreground", "/etc/eye.conf"]
