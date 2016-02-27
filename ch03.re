= 第三章 Ajaxで作る全文検索システム

//lead{
リード
//}

== 環境の準備

Dockerイメージで準備する

== テーブルの作成

Groongaは、全文検索エンジンであると同時にデータベースでもあります。Groongaが検索できる対象はデータベース内に入っているデータに限られますので、まずはデータを投入しましょう。

データを投入するには、データの受け皿となる「テーブル」を作成する必要があります。
テーブルは行と列の組み合わせでデータを保存・表現する方法です。表組みとも言われ、概念的には以下のような物です。

（図）

Microsoft ExcelやGroogle Spreadsheet、MySQLなどのRDBMS、CSV@<fn>{CSV}などでおなじみですね。


//footnote[CSV][Comma-Separated Values（カンマ区切りの値）。一行の中に、カンマで区切った色々な値を書いたテキストファイル。Excelなどのスプレッドシートアプリケーションは多くの場合CSVとしての保存機能を持つ。]
