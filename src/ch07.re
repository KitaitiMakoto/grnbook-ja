= 全文検索機能の作成

ようやく、全文検索機能の実装です。実は、これもMroongaならではの特別なことは必要なく、通常のMySQLの全文検索機能、即ち@<code>{MATCH() ... AGAINST()}構文を使用するだけです。テーブル作成時に、ストレージエンジンにMroongaを選択しておけば、他の操作は通常のMySQLと同じように実行できるようにデザインされているのです。

@<code>{MATCH() ... AGAINST()}構文は元々MySQLに備わっている全文検索用の構文です。@<code>{MATCH}には検索対象にするカラム名を渡します。ここで指定できるのは、テーブル作成時に@<code>{FULLTEXT}インデックスを指定したカラムのみです。Docker内のテーブルでは、予め@<code>{title}カラムと@<code>{content}カラムに@<code>{FULLTEXT}インデックスを張っています。@<code>{AGAINST}には検索クエリーを渡します。オプションを渡すこともできます。詳細は@<href>{https://dev.mysql.com/doc/refman/5.6/ja/fulltext-search.html, MySQLのドキュメント}を参照してください。

Mroongaではこの構文を流用しており、元の挙動を知っている人にはおおよそ期待通りに動作するようになっていますが、厳密に同じではありません。Mroongaでのみ使える様々な機能が使えるようになっています。これについては、一旦基本の検索機能を作ったあとで触れたいと思います。

今の「登録済みPDF一覧」の上に検索フォームを作成し、検索を実行した場合にはフォームの下に検索結果を表示するようにしてみましょう。そのために、@<code>{\PDFSearch\Table}クラスに検索用のメンバー関数を追加します。

//list[ch07/search/search.php][search.php][php]{
#@mapfile(ch07/search/table.php)
#@end
//}

@<code>{SEARCH}定数と@<code>{search}メンバー関数を追加しています。

先ほど言ったような検索フォームと結果のエリアを追加すると、index.phpはこうなります。

//list[ch07/search/index.php][index.php][php]{
#@mapfile(ch07/search/index.php)
#@end
//}

SQL一文で済むので簡単ですね。では、検索してみましょう。

//image[search][全文検索結果の表示]{
//}

以上で、基本的な機能は完成です。以降では、Mroongaに特徴的な幾つかの機能を紹介し、実装していきます。
