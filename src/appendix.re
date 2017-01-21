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

データベースの作成には、通常のMySQLと変わることは何もありません。MySQLサーバーに接続し、@<code>{CREATE DATABASE}文を実行することでデータベースを作成します。

//emlist[データベースの作成][SQL]{
CREATE DATABASE pdfsearch;
//}

必要に応じて権限設定などを行ってください。

テーブルの作成には気を付けることがあります。まず、ストレージエンジンをMroongaにする必要があります。MySQLで何も指定せずに@<code>{CREATE TABLE}を実行した場合、ストレージエンジンはデフォルトのInnoDBになります。これを他のストレージエンジンに変更するには、@<code>{ENGINE}オプションを使用します。

//emlist[テーブルのストレージエンジンにMroongaを指定][SQL]{
CREATE TABLE `pdfs` (
-- カラム定義等
) ENGINE = Mroonga DEFAULT CHARSET utf8;
//}

また、検索対象にしたいカラムに、（通常のインデックスとは異なる）全文検索用インデックスを作成する必要があります。

//emlist[全文検索インデックスの追加][SQL]{
ALTER TABLE `pdfs` ADD FULLTEXT INDEX (title, content);
//}

これ自体は通常のMySQLでの操作なので、@<code>{ALTER TABLE}のほか、@<code>{CREATE TABLE}でインデックスを作成することもできます。

//emlist[テーブル作成時の全文検索インデックスの作成][SQL]{
CREATE TABLE `pdfs` (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    file    VARCHAR(255),
    title   VARCHAR(255),
    content LONGTEXT,
    FULLTEXT INDEX (title, content)
) ENGINE = Mroonga DEFAULT CHARSET utf8;
//}

これは、本書で使用したDockerイメージを作る際に、実際に使用したSQL文です。

また、SQLを使わず@<href>{https://www.phpmyadmin.net/,phpMyAdmin}のUIでテーブルを作成することもあるかも知れません。Mroongaがインストール済みであれば、phpMyAdminでもストレージエンジンの選択肢にMroongaが追加されていることを確認しているので、同じようにMroongaを使い始めることができます。

== Mroongaのコミュニティ、サポート

Mroongaを含むGroongaについては、開発者が日本人であることもあり、日本語で気軽に問い合わせられます。公式サイトにはメーリングリスト、チャット、Twitter、Facebookページについて紹介されているので、質問や要望がある際にはぜひ活用してください。

@<href>{http://mroonga.org/ja/docs/community.html}

また、開発や運用などの有償のサポートサービスもあります。業務で使用する場合には検討するといいでしょう。

@<href>{http://groonga.org/ja/support/}

== Dockerコンテナの作り直し

本書のサンプルではDockerコンテナを使ってApache、PHP、MySQLを動かしています。時には始めからやり直したくなることもあるかも知れません。そうした場合は、以下のように一度コンテナを削除して作り直すことができます。

//emlist[コンテナの削除]{
% docker rm pdfsearch
//}

//emlist[コンテナの作成]{
% docker run --detach --name=pdfsearch --publish=8080:80 \
    --volume=$PWD:/var/lib/pdfsearch kitaitimakoto/grnbook-mroonga
//}

コンテナを削除すると、データベース内のデータは全て失われます。PDFの登録からやり直しとなるので注意しましょう。ホストと共有しているPHPファイルは消えることはありません。

== phpMyAdminによるデータの確認・操作

本書のDockerイメージにはphpMyAdminもインストールされており、@<code>{http://localhost:8080/phpmyadmin}でアクセスできます。名前が@<code>{root}、パスワードはなし、というユーザーが登録されており、全権限を所有しています（PHPで使用しているユーザーと同じです）。

これを使って実際に入っているデータを確認・削除したり、コンテナはそのままでデータベースやテーブルの再作成を行ったりできます。テーブル作成時はストレージエンジンにMroongaを指定することを忘れないようにしてください。詳細は@<hd>{データベースとテーブルの作成}で解説しています。

== PHPのデバッグ

本書サンプルで作成するDockerコンテナの中ではApacheのmod_phpモジュールを使ってPHP 5.6が動作しています。もし、PHPスクリプトがうまく動作しない場合は、Apacheのエラーログを確認するといいでしょう。

まず、@<code>{docker exec}コマンドを使ってコンテナの中に入ります（正確にはコンテナの中でbashを実行します）。

//emlist[docker execでコンテナの中に入る]{
% docker exec --interactive --tty pdfsearch /bin/bash
//}

Apacheのエラーログは/var/log/apache2/error_logなので、これを@<code>{less}や@<code>{tail}などのコマンドで見ながらPHPを実行することで、どんなエラーが起きているのかを知ることができます。

//emlist[ログファイルを監視する]{
# less /var/log/apache2/error_log
# tail -f /var/log/apache2/error_log
//}
