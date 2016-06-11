= データ入力機能の作成

いよいよアプリケーションらしい所を作っていきます。始めから検索機能に着手したいところですが、データがないことには何もできないので、入力機能から作成します。

PDFファイルの内容をMySQLに登録するために、以下のようなステップを踏むことになります。

 1. PHPによるウェブUIから一時ディレクトリーにアップロードする
 2. PDFファイルからタイトルと本文テキストを抽出する
 3. タイトルと本文をMySQLの適切なカラムに挿入する

== PDFのアップロード機能を作る

まずはPDFのアップロード機能を作りましょう。PDFファイルその物は後で使うことはないので、一時ディレクトリーに置いたままで構いません。

ユーザーが表示させる起点の画面をindex.php、そのファイルから呼び出すアップロード機能の部分をupload.phpとして実装することにします。

//list[ch06/upload/index.php][index.php][php]{
#@mapfile(ch06/upload/index.php)
#@end
//}

//list[ch06/upload/upload.php][upload.php][php]{
#@mapfile(ch06/upload/upload.php)
#@end
//}

今の所、単にアップロードされたファイルの名前を表示しているだけです。後でその他の機能を実装していきます。複数のファイルをアップロードすると@<fn>{multi-select}、全てが表示されることも確認してみてください。

//image[upload][ファイルアップロード画面]{
//}

紙幅を減らすため、細かな検証は省略しています。実際に運用するアプリケーションでは安全性やエラーのチェックなどをしっかりと行ってください。

//footnote[multi-select][ファイル選択ダイアログでCtrlキーを押しながらファイルをクリックしていくことで、選んだファイルを同時にアップロードできます。また、Shiftを押しながら最初のファイルと最後のファイルをクリックすることで、その間にあるファイルをすべて選択状態にできます。]

== PDFからの情報を抽出する

次に、PDFファイルからタイトルと本文を抜き出す処理を実装します。これを行うにはいくつか方法がありますが、ここでは@<href>{https://github.com/php-poppler/php-poppler, php-poppler}というライブラリーを使うことにします。php-popplerを使うとPDFを解析してタイトルや本文を抜き出すことができます@<fn>{poppler}。

php-popplerは@<code>{Poppler}名前空間にいくつかのクラスを定義しており、以下のようにして使います。


//emlist[php-popplerの基本的な使い方][php]{
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

//list[ch06/extract/upload.php][upload.php][php]{
#@mapfile(ch06/extract/upload.php)
#@end
//}

実際にPDFファイルをアップロードして、タイトルなどの情報が取得できているか確認しましょう。

//image[extract][PDF内の情報を取得]{
//}

//footnote[poppler][C製のPDFライブラリーでPopplerという物があり、PDFを扱う様々なコマンドラインツールの提供もしています。php-popplerはこれらのコマンド呼び出しをラップしてPHPから扱いやすくした物です。]

== データベースにデータを挿入する

それでは、いよいよMroongaにデータを入力しましょう。と言っても、いつも通り@<code>{PDO}クラスで@<code>{INSERT}文を実行するだけです。Mroongaの方で自動的に検索用のインデックスを作成してくれます。Dockerイメージ内に既にMySQL用のアダプターはインストールされていますので、単にPHPから呼び出すだけで使用できます。

データベースを扱う@<code>{\PDFSearch\Table}クラスを実装し、@<code>{index.php}でそのクラスを使うようにしたのが以下の内容です。@<code>{\PDFSearch\Upload}には変更がないため省略しています。

//list[ch06/insert/index.php][index.php][php]{
#@mapfile(ch06/insert/index.php)
#@end
//}

//list[ch06/insert/table.php][table.php][php]{
#@mapfile(ch06/insert/table.php)
#@end
//}

ウェブブラウザーでアクセスして、データベースの中身を表示してみましょう。

//image[insert][アップロードしたPDFの情報を表示]{
//}
