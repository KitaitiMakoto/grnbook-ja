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

以後、PHPとMySQL、Mroongaを使ってPDF文書の全文検索システムを作成していきますが、普段使っているシステムにインストール済みの物を使うと、バージョンが異なって本書の内容が当てはまらなかったり、また、将来別件で異なるバージョンをインストールしたい場合などに手間が増えてしまいます。設定ファイルの競合も問題になるかも知れません。Dockerを使うと、システムの方に影響を与えずに、本書で必要なバージョンのソフトウェアを揃えることができます。不要になった場合には削除することも簡単で、その場合もシステムの方には影響を及ぼしません。

=== Dockerのインストール

（後で書く）

=== Dockerコンテナの起動

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

=== 動作確認

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

== 作成する全文検索システムの概要

それでは、PHPとMroongaを使って、全文検索システムを作っていきましょう。以下のようなシステムを作ることにします。

: 概要
  登録済みのPDFファイルを全文検索するシステムである
: できること
  検索対象となる文書をウェブUIで登録することができる@<br>{}
  ウェブUI上のテキストフィールドを使って検索することができる
: 実装方針
  ウェブUIはPHPを使用して作成する@<br>{}
  全文検索用のデータはMySQLに格納する@<br>{}
  PDFからテキストを抜き出す処理は本書では扱わない（Dockerイメージ付属のツールを使う）

== データベースの作成

まず、PHPを使って、MySQLにデータベースを作成します。作成の手順は通常のMySQLでのデータベース作成と同様です。Dockerコンテナ内では既にMySQLが動作しており、@<code>{root}ユーザーでパスワード無しでログインできます。

ソースコードは以下のようになります。@<fn>{loosephp}

//list[create-database.php][create-database.php][php]{
#@mapfile(ch02/create-database.php)
#@end
//}

通常のMySQLに於けるデータベースと全く同じ手順でデータベースを作成していることが分かると思います。Mroongaを使う場合にも、データベースの作成には特別なことはありません。

うまくできたら、http://localhost:8080/create-database.phpを表示して、実際に画面上のボタンを押してデータベースを作成してみましょう。「現在のデータベース一覧」に「pdfsearch」というデータベースが加わるはずです。

//footnote[loosephp][限定された状況での一度きりの処理のため、褒められない書き方をしている箇所もあります。実際のアプリケーションで使用する場合には、フレームワークなどのやり方に則った適切な方法でデータベースを操作してください。]

==[column] PHPのデバッグ

PHPが期待通りに動作しない場合や画面が真っ白になって何が悪いか分からない場合は、@<code>{docker logs}コマンドでログを確認することができます。

//list[docker logs][docker logsでPHPのログを確認]{
% docker log pdfsearch
13.03.2016 09:41:25 INFO  -- [web:php] switch :starting [:unmonitored => :starting] (reason: monitor by user)
160313 09:41:26 mysqld_safe Can't log to error log and syslog at the same time.  Remove all --log-error configuration options for --syslog to take effect.
160313 09:41:26 mysqld_safe Logging to '/var/log/mysql/error.log'.
160313 09:41:27 mysqld_safe Starting mysqld daemon with databases from /var/lib/mysql
[Sun Mar 13 09:41:54 2016] 172.17.0.1:58804 [200]: /create-database.php
[Sun Mar 13 09:41:55 2016] 172.17.0.1:58808 [404]: /favicon.ico - No such file or directory
[Sun Mar 13 09:42:01 2016] PHP Parse error:  syntax error, unexpected '$databases' (T_VARIABLE) in /var/lib/pdfsearch/create-database.php on line 14
[Sun Mar 13 09:42:01 2016] 172.17.0.1:58812 [500]: /create-database.php - syntax error, unexpected '$databases' (T_VARIABLE) in /var/lib/pdfsearch/create-database.php on line 14
//}

@<code>{-f}オプションを付け

//emlist[]{
% docker logs -f pdfsearch
//}

と実行すると、ログが出力する度にそのログが表示されます。Dockerコンテナの中では@<href>{http://php.net/manual/ja/features.commandline.webserver.php, PHPのビルトインサーバー}が動作しているため、ログの内容もそれに準じます。

また、ブラウザーでエラー内容を確認したい場合には、PHPファイルの冒頭で

//emlist[][php]{
<?php
ini_set('dispaly_errors', 'stdout');
//}

と設定するとよいでしょう。

== テーブルの作成

データベースが出来たので、以下のカラムを持つ@<code>{pdfs}テーブル作りましょう。

//table[pdfsearch][作成するpdfsテーブル]{
カラム	内容	データ型	備考
----------------------
path	システム内のパス（場所）	VARCHAR(255)	主キー。検索対象ではない
title	PDF文書のタイトル	VARCHAR(255)	検索対象
content	PDF内のテキスト	LONGTEXT	検索対象
//}

やり方はデータベースの作成と同様です。

//list[create-table.php][create-table.php][php]{
#@mapfile(ch02/create-table.php)
#@end
//}

http://localhost:8080/create-table.phpにアクセスして@<code>{pdfs}テーブルを作成してみましょう。事前に@<code>{pdfsearch}テーブルを作っておかないとエラーになるので注意してください。

データベース作成時と違って、Mroongaを使ったテーブルを作る時にはいくつか注意点があります。

一番大事なのは、@<code>{CREATE TABLE}文の最後に、ストレージエンジンとしてMroongaを指定することです。

//emlist[][sql]{
CREATE TABLE (
-- ...
) ENGINE = Mroonga DEFAULT CHARSET utf8;
//}

普段はInnoDBやMyISAMを指定しているところを切り替えるだけなので、難しいことはないでしょう。

次に、インデックスの作成方法がInnoDBなどと異なります。PDFのタイトルや内容を完全一致で検索することはまずないでしょうから、通常のインデックスは必要ありません。代わりに、全文検索のための特別なインデックスを作る、@<code>{FULLTEXT INDEX}構文を使用します。これは、MySQLに元々備わっている、全文検索機能のための構文です。Mroongaでも用途は同じなので、全く同じ構文で使用できます。

それ以外は、通常のMySQL使い方と同じです。主キーにはパスといった意味のある物ではなく、ただレコードを識別するためだけのサロゲートキーを使う人もいることでしょう。それでも構いません。

== データ入力機能の作成

いよいよアプリケーションらしい所を作っていきます。始めから検索機能に着手したいところですが、データがないことには何もできないので、入力機能から作成します。

PDFファイルの内容をMySQLに登録するために、以下のようなステップを踏むことになります。

 1. PHPによるウェブUIから一時ディレクトリーにアップロードする
 2. PDFファイルからタイトルと本文テキストを抽出する
 3. タイトルと本文をMySQLの適切なカラムに挿入する

=== PDFのアップロード機能を作る

まずはPDFのアップロード機能を作りましょう。PDFファイルその物は後で使うことはないので、一時ディレクトリーに置いたままで構いません。

ユーザーが表示させる起点の画面をindex.php、そのファイルから呼び出すアップロード機能の部分をupload.phpとして実装することにします。

//list[index.php][index.php][php]{
#@mapfile(ch02/upload/index.php)
#@end
//}

//list[upload.php][upload.php][php]{
#@mapfile(ch02/upload/upload.php)
#@end
//}

今の所、単にアップロードされたファイルの名前を表示しているだけです。後でその他の機能を実装していきます。複数のファイルをアップロードすると@<fn>{multi-select}、全てが表示されることも確認してみてください。

//image[upload][ファイルアップロード画面]{
//}

紙幅を減らすため、細かな検証は省略しています。実際に運用するアプリケーションでは安全性やエラーのチェックなどをしっかりと行ってください。

//footnote[multi-select][ファイル選択ダイアログでCtrlキーを押しながらファイルをクリックしていくことで、選んだファイルを同時にアップロードできます。また、Shiftを押しながら最初のファイルと最後のファイルをクリックすることで、その間にあるファイルをすべて選択状態にできます。]

=== PDFからの情報を抽出する

次に、PDFファイルからタイトルと本文を抜き出す処理を実装します。これを行うにはいくつか方法がありますが、ここでは@<href>{https://github.com/php-poppler/php-poppler, php-poppler}というライブラリーを使うことにします。php-popplerを使うとPDFを解析してタイトルや本文を抜き出すことができます@<fn>{poppler}。

php-popplerは@<code>{Poppler}名前空間にいくつかのクラスを定義しており、以下のようにして使います。


//list[php-poppler][php-popplerの基本的な使い方][php]{
<?php

// pdfinfoコマンドのラッパー
$pdfinfo = \Poppler\Driver\Pdfinfo::create();
// pdftotextコマンドのラッパー
$pdftotext = \Poppler\Driver\Pdftotext::create();
// pdftohtmlコマンドのラッパー
$pdftohtml = \Poppler\Driver\Pdftohtml::create();

$pdf = new \Poppler\Processer\Pdffile($pdfinfo, $pdftotet, $peftohtml);

// PDFからタイトルや著者、作成日時などの情報を連想配列として抜き出す
echo $pdf->getInfo('path/to/document.pdf');
// PDFから本文テキストを抜き出す
echo $pdf->toText('path/to/document.pdf');
//}

HTML関連の機能を使わないとしても、@<code>{\Poppler\Processer\Pdffile}のコンストラクターには@<code>{\Poppler\Driver\Pdftohtml}のインスタンスを渡す必要があるので注意してください。

Dockerイメージの中には、既にPopplerとphp-popplerがインストール済みです。php-popplerはComposerでインストールしているので、@<code>{vendor/autoload.php}を読み込むことで、オートロードが有効になります。

これを使って、先ほど作った@<code>{\PDFSearch\Upload}クラスにPDF関連の機能を追加します。

//list[ch02/extract/upload.php][upload.php][php]{
#@mapfile(ch02/upload/upload.php)
#@end
//}

実際にPDFファイルをアップロードして、タイトルなどの情報が取得できているか確認しましょう。

//image[extract][PDF内の情報を取得]{
//}

//footnote[poppler][C製のPDFライブラリーでPopplerという物があり、PDFを扱う様々なコマンドラインツールの提供もしています。php-popplerはこれらのコマンド呼び出しをラップしてPHPから扱いやすくした物です。]

=== データベースにデータを挿入する

それでは、いよいよMroongaにデータを入力しましょう。と言っても、いつも通り@<code>{PDO}クラスで@<code>{INSERT}文を実行するだけです。Mroongaの方で自動的に検索用のインデックスを作成してくれます。Dockerイメージ内に既にMySQL用のアダプターはインストールされていますので、単にPHPから呼び出すだけで使用できます。

データベースを扱う@<code>{\PDFSearch\Table}クラスを実装し、@<code>{index.php}でそのクラスを使うようにしたのが以下の内容です。@<code>{\PDFSearch\Upload}には変更がないため省略しています。

//list[ch02/insert/index.php][index.php][php]{
#@mapfile(ch02/insert/index.php)
#@end
//}

//list[ch02/insert/table.php][table.php][php]{
#@mapfile(ch02/insert/table.php)
#@end
//}

ウェブブラウザーでアクセスして、データベースの中身を表示してみましょう。

//image[insert][アップロードしたPDFの情報を表示]{
//}

== 全文検索機能の作成

ようやく、全文検索機能の実装です。実は、これもMroongaならではの特別なことは必要なく、通常のMySQLの全文検索機能、即ち@<code>{MATCH() ... AGAINST}構文を使用するだけです。テーブル作成時に、ストレージエンジンにMroongaを選択しておけば、他の操作は通常のMySQLと同じように実行できるようにデザインされているのです。

@<code>{MATCH() ... AGAINST}構文は元々MySQLに備わっている全文検索用の構文です。@<code>{MATCH}には検索対象にするカラム名を渡します。ここで指定できるのは、テーブル作成時に@<code>{FULLTEXT}インデックスを指定したカラムのみです。Docker内のテーブルでは、予め@<code>{title}カラムと@<code>{content}カラムに@<code>{FULLTEXT}インデックスを張っています。@<code>{AGAINST}には検索クエリーを渡します。オプションを渡すこともできます。詳細は@<href>{https://dev.mysql.com/doc/refman/5.6/ja/fulltext-search.html, MySQLのドキュメント}を参照してください。

Mroongaではこの構文を流用しており、元の挙動を知っている人にはおおよそ期待通りに動作するようになっていますが、厳密に同じではありません。Mroongaでのみ使える様々な機能が使えるようになっています。これについては、一旦基本の検索機能を作ったあとで触れたいと思います。

今の「登録済みPDF一覧」の上に検索フォームを作成し、検索を実行した場合にはフォームの下に検索結果を表示するようにしてみましょう。そのために、@<code>{\PDFSearch\Table}クラスに検索用のメンバー関数を追加します。

//list[ch02/search/search.php][search.php][php]{
#@mapfile(ch02/search/table.php)
#@end
//}

@<code>{SEARCH}定数と@<code>{search}メンバー関数を追加しています。

先ほど言ったような検索フォームと結果のエリアを追加すると、index.phpはこうなります。

//list[ch02/search/index.php][index.php][php]{
#@mapfile(ch02/search/index.php)
#@end
//}

SQL一文で済むので簡単ですね。では、検索してみましょう。

//image[search][全文検索結果の表示]{
//}

以上で、基本的な機能は完成です。以降では、Mroongaに特徴的な幾つかの機能を紹介し、実装していきます。
