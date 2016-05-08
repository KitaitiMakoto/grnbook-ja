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
    gem install eye && \
    php -r "readfile('https://getcomposer.org/installer');" > composer-setup.php && \
    php -r "if (hash('SHA384', file_get_contents('composer-setup.php')) === '7228c001f88bee97506740ef0888240bd8a760b046ee16db8f4095c0d8d525f2367663f22a46b48d072c816e7fe19959') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;" && \
    php composer-setup.php --install-dir=/usr/local/bin --filename=composer && \
    rm composer-setup.php && \
    COMPOSER_HOME=/usr/share/php composer global require 'php-poppler/php-poppler'
RUN mysqld & sleep 12 && \
    mysql -uroot -e "INSTALL PLUGIN mroonga SONAME 'ha_mroonga.so'" && \
    mysql -uroot -e "CREATE FUNCTION mroonga_snippet_html RETURNS STRING SONAME 'ha_mroonga.so'" && \
    mysqladmin -uroot shutdown

COPY eye.conf /etc/
COPY eye /etc/eye/
COPY 000-default.conf /etc/apache2/sites-available/
COPY pdfsearch.conf /etc/apache2/sites-available/
RUN ln -s /etc/apache2/sites-available/pdfsearch.conf /etc/apache2/sites-enabled/

COPY php.ini /etc/php5/apache2/
COPY config.inc.php /etc/phpmyadmin/config.inc.php

EXPOSE 80

CMD ["eye", "load", "--foreground", "/etc/eye.conf"]
