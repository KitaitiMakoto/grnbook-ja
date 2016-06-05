= Mroongaの環境の準備

本書では、MySQL及びその他の環境の準備にDockerを使います。

以後、PHPとMySQL、Mroongaを使ってPDF文書の全文検索システムを作成していきますが、普段使っているシステムにインストール済みの物を使うと、バージョンが異なって本書の内容が当てはまらなかったり、また、将来別件で異なるバージョンをインストールしたい場合などに手間が増えてしまいます。設定ファイルの競合も問題になるかも知れません。Dockerを使うと、システムの方に影響を与えずに、本書で必要なバージョンのソフトウェアを揃えることができます。不要になった場合には削除することも簡単で、その場合もシステムの方には影響を及ぼしません。

== Dockerのインストール

（後で書く）

== Dockerコンテナの起動

本書用に必要なソフトウェアをインストールしたDockerイメージを作成してあります。ターミナルで上の@<code>{docker run}を実行したディレクトリーに移動し、以下のコマンドを実行してください。

//list[docker run][Dockerイメージの取得とコンテナの起動]{
% cd path/to/project
% docker run --detach --name=pdfsearch --publish=8080:80 \
    --volume=$PWD:/var/lib/pdfsearch kitaitimakoto/grnbook-mroonga
//}

@<code>{docker run}コマンドにより、以下の三つのことが行われます。

 1. インターネット経由のDockerイメージの取得（もしシステム上に存在しない場合）
 2. 取得したDockerイメージを元にしたDockerコンテナの作成
 3. 作成したDockerコンテナの起動

Dockerイメージの取得とコンテナの作成は一度行えば充分なので、コンテナの二度目以降の起動には別のコマンドを使用します。

//emlist[Dockerコンテナの起動]{
% docker start pdfsearch
//}

//emlist[Dockerコンテナの停止]{
% docker stop pdfsearch
//}

コンテナの操作に使用している@<code>{pdfsearch}という名前は、@<list>{docker run}における@<code>{name}オプションで指定した物を使用しています。@<code>{docker run}実行時に異なる名前を指定した場合は、そちらを使いましょう。（nameをつけ忘れた時はdocker psで探して消すが、説明いる？）

もし、二度目も@<code>{docker run}コマンドを使用してしまうと、新たに@<em>{別の}コンテナを作成してしまいます。MySQLのデータ等は引き継がれませんし、同じ目的のコンテナが二つ以上あると混乱の元ですので、通常は一つになるようにしておきましょう。

== 動作確認

Dockerコンテナが実際に動作していることを確認しましょう。このイメージにはPHPが含まれているので、@<code>{docker run}を実行したディレクトリーで以下のようなファイルを作成することで動作確認ができます。

//list[info.php][info.php][php]{
<?php
phpinfo();

//}

ブラウザーでhttp://localhost:8080/info.phpにアクセスしてください。PHPの情報が表示されれば、環境の準備は成功しています。
（Docker Toolbox使っている場合はlocalhostじゃなさそう）

「Not Found」の画面が表示されてしまう場合は、どこかで間違えてしまったようです。これまでの手順を見ながらやり直してみてください。

@<code>{docker run}をやり直すには、以下のコマンドを実行して一旦Dockerコンテナを停止し、削除する必要があります。

//emlist[Dockerコンテナの停止と削除]{
% docker stop pdfsearch
% docker rm pdfsearch
//}

ありがちな間違いとして、Dockerコンテナとの共有ディレクトリーの指定ミスがあります。Dockerコンテナとホストマシンは、@<code>{docker run}コマンドの@<code>{volume}（@<code>{v}）オプションで指定したディレクトリーを共有します。上の例では@<code>{$PWD:/var/lib/pdfsearch}を指定しています。コロンの左側がホストマシンのディレクトリー、右側がコンテナ内のディレクトリーです。@<code>{$PWD}は（@<code>{docker run}を実行した）現在のディレクトリーを意味しますので、そこと別のディレクトリーにファイルを置いた場合は、コンテナ内のPHPが認識できません。事前にプロジェクトのディレクトリーに移動してから実行するようにしましょう（コンテナ内は@<code>{/var/lib/pdfsearch}がドキュメントルートとなるよう設定されており、こちらは常にこの値で大丈夫です）。
