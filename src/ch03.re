= Ajaxで作る全文検索システム

//lead{
リード
//}

== 環境の準備

Dockerイメージで準備する

ポート番号の注意を書く。既に使っているかも知れない。エラーメッセージ付きがいいだろう。

確認済みブラウザーとか書く

この章で作る全文検索システムについて書く

== テーブルの作成

Groongaは、全文検索エンジンであると同時にデータベースでもあります。Groongaが検索できる対象はデータベース内に入っているデータとなるので、まずはデータを投入しましょう。

データを投入するには、データの入れ物となる「テーブル」を作成する必要があります。テーブルは行と列の組み合わせでデータを保存・表現する方法です。概念的には以下のような物です。

//table[tablesample][テーブルの概要]{
名前	パス	内容
--------------
CMS操作マニュアル	/Users/me/Documents/CMSマニュアル.pdf	このマニュアルでは...
経営状況報告	/Users/me/Documents/経営状況報告2016.pdf	我社の昨年の経営状況は...
//}

Microsoft ExcelやGroogle Spreadsheet、MySQLなどのRDBMS、CSV@<fn>{CSV}などでおなじみですね。上の例でも分かるように、テーブルには「名前」「パス」「内容」といった「どんな内容のデータが入るのか」ということを、列として予め定義しておく必要があります。データの投入時にはこの定義に従ってデータを整理した上で投入することになります。なお、Groongaでは列をカラムと呼ぶことがほとんどなので、ここでもカラムという語を使うことにします。

Groongaでテーブルを作成してこうしたカラム定義を行うには、HTTP経由で@<code>{table_create}と@<code>{column_create}というコマンドを実行します@<fn>{commandname}。HTTPなので、AjaxでGroongaを操作できるということです。

テーブルの準備に時間を掛けても飽きてしまうので、詳しい説明は後に回すことにして、ここでは実際にテーブルを作るまでを駆け足でやっていきたいと思います。

//footnote[CSV][Comma-Separated Values（カンマ区切りの値）。一行の中に、カンマで区切った色々な値を書いたテキストファイル。Excelなどのスプレッドシートアプリケーションは多くの場合CSVとしての保存機能を持つ。]
//footnote[commandname][コマンド名が通常の英語の順番（create table）と反対であることに気付いたでしょうか。Groongaでは何についての操作なのか＋どういう操作なのかという順番でコマンド名が決められています。日本語の目的語＋動詞（テーブルを＋作る）という順番と同じですね。]

=== 作成する画面

テーブルの作成は一度行えば二度は必要ないので、後で作る検索アプリケーションとは分けたページにします。画面の構成要素はボタン一つだけで、このボタンを押すとGroongaデータベースに@<code>{Documents}というテーブルと@<code>{Terms}というテーブルを作るようにします。一度テーブルを作った後は、ボタンを無効化します。

//image[table-creation-initial][テーブル作成前の表示]{
//}
//image[table-creation-done][テーブル作成後の表示]{
//}
//image[table-creation-existing][既にテーブルが存在している場合の表示]{
//}

=== 実装

init.html、init.jsという名前のファイルを作成してください。内容はそれぞれ以下の通りです。

//list[init.html][init.html][html]{
#@mapfile(ch03/init.html)
#@end
//}

//list[init.js][init.js][javascipt]{
#@mapfile(ch03/init.js)
#@end
//}

（長い……）
（Promiseは敷居が高いだろうか？　jQueryとかって今でも共通言語？）

http://localhost:3000/app/init.htmlにアクセスすると、「データベースの作成」ボタン一つだけの画面が表示されるはずです（ポート番号の3000の部分は必要に応じて読み替えてください）。ボタンを押すことで、本章で必要なテーブルを作成することができます@<fn>{createtables}。

非同期処理のハンドリングの記述で長くなっていますが、主に次のことを行っています。

 1. テーブル一覧を取得し、@<code>{Documents}というテーブルと@<code>{Terms}というテーブルが存在しているか確認する
 2. 存在していれば、それ以上何もしない
 3. 存在していなければ、ボタンのクリックイベントにテーブル作成処理を割り当てる

ボタンのクリックイベントには次の処理を割り当てています。

 1. @<code>{Documents}テーブルを作成する
 2. @<code>{Documents}テーブルに@<code>{title}というカラムを定義する
 3. @<code>{Documents}テーブルに@<code>{content}というカラムを定義する
 4. @<code>{Terms}テーブルを作成する
 5. @<code>{Terms}テーブルに@<code>{content_terms}カラムを定義する

細かいパラメーターの意味はここでは気にしないで構いません。後で解説します。

なお、groonga-httpdにはウェブブラウザーで操作できるUIが同梱されており、http://localhost:10041/でアクセスできます。「List table」をクリックすることでテーブル一覧を表示することができ、テーブルにチェックをして「Remove table」をクリックすると、テーブルを削除できます。全てを最初からやり直したい、JavaScriptを自分で修正しつつ試したい、といった場合はここで削除しながら進めるといいでしょう。

//image[admin-table-deletion][管理画面でテーブルを削除することができる][scale=1]{
//}

//footnote[createtables][正確には、テーブルを作成し、その上で必要なカラムを定義しています。]

====[column] Ajaxのクロスオリジンアクセス

init.htmlはhttp://localhost:3000で動作しているのに、Ajaxでhttp://localhost:10041と通信している、つまり同一オリジンポリシーに違反しているのに、なぜ動作しているのか、不思議に思った人もいるかも知れません。

これを可能にするために、Dockerイメージ内のgroonga-httpdではCORSの設定を行っています。CORS（Cross Origin Resource Sharing）はその名の通りオリジンが違う環境でリソース（この場合はGroongaの検索結果）を共有するための規約で、サーバーを適切に設定することで有効にできます。詳しくは以下の記事などを参照してください。

@<href>{https://developer.mozilla.org/ja/docs/HTTP_access_control, HTTP アクセス制御 (CORS) | MDN}

公開サーバーでgroonga-httpdを使ってアプリケーションを作る時にはこの設定をしないと動作しないこともあるので、うまく動かない時は思い出しましょう。

== PDFデータの投入
