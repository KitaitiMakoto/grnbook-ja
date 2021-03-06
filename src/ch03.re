= 作成する全文検索システムの概要

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
