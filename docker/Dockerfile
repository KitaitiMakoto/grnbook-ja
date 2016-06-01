FROM ruby:2.3
MAINTAINER KITAITI Makoto

COPY groonga.list /etc/apt/sources.list.d

RUN apt-get update && \
    apt-get install -y --allow-unauthenticated groonga-keyring && \
    apt-get update && \
    { echo mysql-server mysql-server/root-password password 'root'; \
      echo mysql-server mysql-server/root_password_again password 'root'; \
    } | debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server poppler-utils php5-cli && \
    apt-get clean && \
    gem update --system && \
    gem update --no-document && \
    gem cleanup && \
    gem install eye --no-document && \
    php -r "readfile('https://getcomposer.org/installer');" > composer-setup.php && \
    php -r "if (hash_file('SHA384', 'composer-setup.php') === '070854512ef404f16bac87071a6db9fd9721da1684cd4589b1196c3faf71b9a2682e2311b36a5079825e155ac7ce150d') { echo 'Installer verified'; } else { echo 'Installer corrupt'; unlink('composer-setup.php'); } echo PHP_EOL;" && \
    php composer-setup.php --install-dir=/usr/local/bin --filename=composer && \
    rm composer-setup.php && \
    COMPOSER_HOME=/usr/share/php composer global require 'php-poppler/php-poppler'
RUN mysqld & sleep 12 && \
    { echo phpmyadmin phpmyadmin/reconfigure-webserver multiselect none; \
    } | debconf-set-selections && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y mysql-server-mroonga phpmyadmin && \
    mysqladmin -uroot shutdown && \
    apt-get clean

COPY eye.conf /etc/
COPY eye /etc/eye/
COPY 000-default.conf /etc/apache2/sites-available/
COPY pdfsearch.conf /etc/apache2/sites-available/
RUN ln -s /etc/apache2/sites-available/pdfsearch.conf /etc/apache2/sites-enabled/

COPY php.ini /etc/php5/apache2/
COPY config.inc.php /etc/phpmyadmin/config.inc.php

EXPOSE 80

CMD ["eye", "load", "--foreground", "/etc/eye.conf"]
