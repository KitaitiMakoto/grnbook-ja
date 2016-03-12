= MySQLで作る全文検索システム

//lead{
Groongaは様々なソフトウェアに組み込み、全文検索機能を提供することができます。その一つ、MySQLに組み込むためのMroongaを使って、全文検索システムを作ってみましょう。
//}

== Mroongaの概要

Mroongaは、MySQLのストレージエンジンの一つです@<fn>{mariadb}。MySQLのストレージエンジンにはMyISAMやInnoDBなどがあり、聞いたことがある人も多いでしょう。それぞれにパフォーマンスや対障害性などに特徴があり、テーブルごとに使い分けることができます。

//footnote[mariadb][MySQLから派生したMariaDBというデータベースシステムでも使用できます。最新のMariaDBでは始めからMroongaが組み込まれています。]

その一つとしてMroongaは、Groongaを組み込んで、高速かつ多機能な全文検索を可能にしたストレージエンジンです。これをMySQLに組み込んで使うことで、SQLを使って全文検索を行えるようになります。

Mroongaを全文検索システムの本番環境で使うには、公式マニュアルのインストールのページを参照してください。ここでは、本書で試す目的で、簡易に環境を準備する方法を紹介します。

== Mroongaの環境の準備

本書では、MySQL及びその他の環境の準備にDockerを使います。

=== Dockerのインストール

（後で書く）

=== Dockerコンテナの起動

本書用に必要なソフトウェアをインストールしたDockerイメージを作成してあります。ターミナルで以下のコマンドを実行してください。

//list[docker run][Dockerイメージの取得とコンテナの起動]{
% cd path/to/project
% docker run --detach --name=grnbook --publish=8080:80 --volume=$PWD:/var/lib/grnbook kitaitimakoto/grnbook-mroonga
//}

@<code>{docker run}コマンドを実行すると、以下の二つのことが行われます。

 1. Dockerイメージの取得（もしシステム上に存在しない場合）
 2. 取得したDockerイメージを元にしたDockerコンテナの起動

Dockerイメージの取得は一度行えば充分なので、コンテナの二度目以降の起動には別のコマンドを使用します。

//emlist[Dockerコンテナの起動]{
% docker start grnbook
//}

//emlist[Dockerコンテナの停止]{
% docker stop grnbook
//}

コンテナの操作に使用している@<code>{grnbook}という名前は、@<list>{docker run}における@<code>{name}オプションで指定した物を使用します。

=== 動作確認

Dockerコンテナが実際に動作していることを確認しましょう。このイメージにはPHPが含まれているので、以下のようなファイルを作成することで動作確認ができます。

//list[info.php][info.php][php]{
<?php
phpinfo();

//}

ブラウザーでhttp://localhost:8080/info.phpにアクセスしてください。以下のようにPHPの情報が表示されれば、環境の準備は成功しています。

//image[phpinfo][phpinfo()の実行結果][scale=1]{
//}

「Not Found」の画面が表示されてしまう場合は、どこかで間違えてしまったようです。これまでの手順を見ながらやり直してみてください。

@<code>{docker run}をやり直すには、以下のコマンドを実行して一旦Dockerコンテナを削除する必要があります。

//emlist[Dockerコンテナの削除]{
% docker rm grnbook
//}

ありがちな間違いとして、Dockerコンテナと@<code>{docker}コマンドを実行している（OS Xなどの）ホストマシンとで共有するディレクトリーをうまく設定できないことがあります。@<list>{docker run}で@<code>{volume}オプションに@<code>{$PWD}を使用していますが、これは現在のディレクトリーを意味します。そのため、このコマンドを実行した時のディレクトリーが、PHPファイルを措くべき場所として使用されます。事前のプロジェクトのディレクトリーに移動してから実行するようにしましょう。
