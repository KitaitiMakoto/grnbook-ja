= 付録

== Mroongaのインストール

Mroongaを使った全文検索システムに興味が出て来たら、ぜひ自分でインストールしてみてください。以下に、典型的なインストール方法をご案内します。

=== Debian/GNU Linux

Groongaプロジェクト（？）は、本書執筆時点でDebian/GNU Linux jessieのAPT用のリポジトリーを持っており、Mroongaもそこからインストールできます。サンプルで用いたDockerイメージでもこのパッケージを使用しています。

まず、以下の内容で/etc/apt/sources.list.d/groonga.listというを作成し、ソースを登録します。

//emlist[/etc/apt/sources.list.d/groonga.list]{
deb http://packages.groonga.org/debian/ jessie main
deb-src http://packages.groonga.org/debian/ jessie main
//}

次にインストールします。

//emlist{
% sudo apt-get update
% sudo apt-get install -y --allow-unauthenticated groonga-keyring
% sudo apt-get update
% sudo apt-get install -y -V mysql-server-mroonga
//}

以上でインストールは終了です。

実際にインストールする際には細部が変わっている可能性もあるため、一度以下の公式ドキュメントを確認しましょう。

@<href>{http://mroonga.org/ja/docs/install/debian.html}

=== Windows、OS X、Ubuntu、CentOS

Windows用にも公式にMroongaのインストーラー及びコンパイル済みパッケージのZIPアーカイブが提供されています。OS XのHomebrew、UbuntuのAPT、CentOSのYum用にはリポジトリーが提供されています。

いずれも公式サイトにインストール方法がありますので、参照してください。

@<href>{http://mroonga.org/ja/docs/install.html}

=== Docker

MroongaのDockerイメージもDocker Hubから配布されています。以下のように@<code>{docker run}コマンドを実行することで、自動的にイメージの取得からコンテナの起動までが行われます。

//emlist[MroongaのDockerイメージの取得]{
% docker run --detach --publish=3306:3306 groonga/mroonta
//}

これでポート番号3306をリスンしながらMroongaインストール済みのMySQLサーバーが起動します。通常のMySQLクライアントを使って接続が可能です。

//emlist[Mroongaへの接続]{
% mysql --host=127.0.0.1 --port=3306 --user=root
//}

（Linux以外は--hostが違いそう）

但し、Dockerコンテナは、削除と同時にデータが失われてしまいます。これは多くの場合望ましい挙動ではないでしょう。次のように@<code>{volume}オプションを追加することで、ホスト上のディレクトリーをコンテナと共有できます。

//emlist[volumeオプションの指定]{
% docker run --detach --publish=3306:3306 --volume=/tmp/mroonga:/var/lib/mysql groonga/mroonga
//}

これで、 ホスト側の/tmp/mroonga以下にMySQLのデータファイルが作成されるようになります。

Mroongaを使ったテーブルの作成については、@<hd>{データベースとテーブルの作成}を参照してください。

また、MroongaのDockerイメージは、MySQLとMroongaそれぞれのバージョンの組み合わせごとに多数のイメージが作られています。入手可能な組み合わせはDocker HubのMroongaのページで確認できます。

@<href>{https://hub.docker.com/r/groonga/mroonga/}

== データベースとテーブルの作成

== Dockerコンテナの作り直し

== phpMyAdminによるデータの確認・操作

== PHPのデバッグ
