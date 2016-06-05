= テーブルの作成

データベースが出来たので、以下のカラムを持つ@<code>{pdfs}テーブル作りましょう。

//table[pdfsearch][作成するpdfsテーブル]{
カラム	内容	データ型	備考
----------------------
path	システム内のパス（場所）	VARCHAR(255)	主キー。検索対象ではない
title	PDF文書のタイトル	VARCHAR(255)	検索対象
content	PDF内のテキスト	LONGTEXT	検索対象
//}

やり方はデータベースの作成と同様です。

//list[ch05/create-table.php][create-table.php][php]{
#@mapfile(ch05/create-table.php)
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
