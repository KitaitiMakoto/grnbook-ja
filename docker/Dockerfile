FROM ruby:2.3
MAINTAINER KITAITI Makoto

COPY groonga.list /etc/apt/sources.list.d

RUN apt-get update && \
    apt-get install -y --allow-unauthenticated groonga-keyring && \
    apt-get update && \
    { echo mysql-server mysql-server/root-password password ''; \
      echo mysql-server mysql-server/root_password_again password ''; \
      echo phpmyadmin phpmyadmin/dbconfig-install boolean false; \
      echo phpmyadmin phpmyadmin/reconfigure-webserver multiselect apache2; \

      echo phpmyadmin phpmyadmin/app-password-confirm password ''; \
      echo phpmyadmin phpmyadmin/mysql/admin-pass password ''; \
      echo phpmyadmin phpmyadmin/password-confirm password ''; \
      echo phpmyadmin phpmyadmin/setup-password password ''; \
      echo phpmyadmin phpmyadmin/database-type select mysql; \
      echo phpmyadmin phpmyadmin/mysql/app-pass password ''; \
    } | debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server-mroonga groonga-tokenizer-mecab phpmyadmin poppler-utils && \
    gem update --system && \
    gem update && \
    gem install eye
RUN mysqld & sleep 12 && mysql -uroot -e "INSTALL PLUGIN mroonga SONAME 'ha_mroonga.so'" && mysqladmin -uroot shutdown

COPY eye.conf /etc/
COPY eye /etc/eye/

EXPOSE 80

CMD ["eye", "load", "--foreground", "/etc/eye.conf"]
