= Mroongaの概要

Mroongaは、MySQLのストレージエンジンの一つです@<fn>{mariadb}。MySQLのストレージエンジンにはMyISAMやInnoDBなどがあり、聞いたことがある人も多いでしょう。それぞれにパフォーマンスや耐障害性などに特徴があり、テーブルごとに使い分けることができます。

その一つとしてMroongaは、Groongaを組み込んで、高速かつ多機能な全文検索を可能にしたストレージエンジンです。これをMySQLに組み込んで使うことで、SQLを使って全文検索を行えるようになります。

Mroongaを全文検索システムの本番環境で使うには、@<href>{http://mroonga.org/ja/docs/install.html, 公式マニュアルのインストールのページ}を参照してください。ここでは、本書で試す目的で、簡易に環境を準備する方法を紹介します。

//footnote[mariadb][MySQLから派生したMariaDBというデータベースシステムでも使用できます。最新のMariaDBでは始めからMroongaが組み込まれています。]
