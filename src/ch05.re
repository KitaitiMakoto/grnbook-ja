= 全文検索機能の作成

ようやく、全文検索機能の実装です。実は、これもMroongaならではの特別なことは必要なく、MySQLの通常の@<code>{MATCH(...) AGAINST(...)}構文を使用するだけです。テーブル作成時に、ストレージエンジンにMroongaを選択しておけば、他の操作は通常のMySQLと同じように実行できるようデザインされているのです。

@<code>{MATCH(...) AGAINST(...)}構文は元々MySQLに備わっている全文検索用の構文です。@<code>{MATCH()}には検索対象にするカラム名を渡します。ここで指定できるのは、テーブル作成時に@<code>{FULLTEXT}インデックスを指定したカラムのみです。Docker内のテーブルでは、予め@<code>{title}カラムと@<code>{content}カラムのセットに@<code>{FULLTEXT}インデックスを張っています。@<code>{AGAINST()}には検索クエリーを渡します。オプションを渡すこともできます。詳細は@<href>{https://dev.mysql.com/doc/refman/5.6/ja/fulltext-search.html, MySQLのドキュメント}を参照してください。

Mroongaではこの構文を流用しており、元の挙動を知っている人にはおおよそ期待通りに動作するようになっていますが、厳密に同じではありません。Mroongaでのみ使える様々な機能が使えるようになっています。これについては、一旦基本の検索機能を作ったあとで触れたいと思います。

== 一検索語用の実装


今の「登録済みPDF一覧」の上に検索フォームを作成し、検索を実行した場合にはフォームの下に検索結果を表示するようにしてみましょう。そのために、@<code>{\PDFSearch\Table}クラスに検索用のメンバー関数を追加します。

//list[ch05/search/table.php][table.php][php]{
<?php
#@maprange(ch05/search/table.php,head)
#@end
    // :
    // :
#@maprange(ch05/search/table.php,search)
#@end
//}

@<code>{SEARCH}定数と@<code>{search}メンバー関数を追加しています。

先ほど言ったような検索フォームと結果のエリアを追加すると、index.phpはこうなります。

//list[ch05/search/index.php][index.php][php]{
<?php
// :
// :
#@maprange(ch05/search/index.php,search)
#@end
// :
// :
#@maprange(ch05/search/index.php,searchresult)
#@end
//}

SQL一文で済むので簡単ですね。では、検索してみましょう。

//image[search][全文検索結果の表示]{
//}

以上で、基本的な機能は完成です。

== AND検索の実装

一単語で検索している間は気になりませんが、検索フォームに二単語入れると、検索結果に違和感を覚えるかも知れません。例えば「Commons ライセンス」で検索した場合に、「『Commons』は含んでいるが、『ライセンス』は含んでいない」という文書もヒットする（OR検索になっている）ためです。こういう結果が望ましい場面もありますが、Google検索はこうではなく「Commons ライセンス」で検索した場合には「『Commons』も『ライセンス』も両方とも含んでいるページ」が検索結果上位に登場するようになっています（AND検索になっている）。Google検索には非常にたくさんの人が馴染んでいるため、どちらの挙動でもいい場合には合わせておくのがいいでしょう。

Mroongaでこのような挙動になっているのは、MySQLのブーリアンモードによる全文検索が、デフォルトでこのようになっているためです。AND検索になるよう、何らかの処理を追加する必要があります。

方法は二つあり、一つは、PHPでクエリーパラメーター（@<code>{$_GET['q']}）を単語に分割し、それぞれにAND検索用の命令を追加していく方法です。Mroongaに限らずMySQLの全文検索でも使える汎用的な方法ですが、SQLインジェクション脆弱性などを作り込まずにPHPを書いていく必要があり、本書のスコープ外になるためここでは採用しません。

もう一つは、SQLのクエリーを実行するタイミングで、クエリー内容の解釈を変更させる「@<kw>{プラグマ}」というMroonga特有の命令を使う方法です。プラグマは@<code>{AGAINST()}句内で使用し、次のようになります。

//emlist[プラグマを追加したクエリー][SQL]{
AGAINST('*D+ Commons ライセンス' IN BOOLEAN MODE)
//}

検索語の先頭に追加されたのがプラグマです。以下のような構文になっています。

 * 「@<code>{*}」で開始する
 * プラグマの種類を指定する。種類には「@<code>{D}」や「@<code>{W}」などがある
 * 種類の後に、そのプラグマの詳細な挙動を指定するパラメーターを指定する（指定方法はプラグマごとに異なる）。ここでは「@<code>{+}」を指定している
 * 検索語の前に空白文字を置く

検索をAND検索にするには「@<code>{D+}」というプラグマを使用します。「@<code>{D}」によって「複数の検索語をANDで扱うのかORなのかNOTなのか」という検索語の扱いを指定する、と宣言し、「@<code>{+}」を使うことで「AND検索として扱う」ということを指定します。「@<code>{+}」の部分を「@<code>{-}」など他のキーワードに変えることで別の指定もできますが、ここでは扱いません。

本書の範囲を外れる詳細については、Mroongaの@<href>{http://mroonga.org/ja/docs/reference/full_text_search/boolean_mode.html, ブーリアンモード}のマニュアルページを参照してください。

それでは実際に、これを使ってAND検索を実装しましょう。@<code>{\PDFSearch\Table\SEARCH}定数のSQLと、その中のパラメーターをバインドする変数を変更します。

//list[ch05/pragma/table.php][table.php][php]{
<?php
#@maprange(ch05/pragma/table.php,head)
#@end
    // :
    // :
#@maprange(ch05/pragma/table.php,sql)
#@end
    // :
    // :
#@maprange(ch05/pragma/table.php,search)
#@end
    // :
    // :
//}

これで「Commons ライセンス」と検索した場合に、両方を含む文書しかヒットしないようになります。

次章では、Mroongaに特徴的な幾つかの機能を紹介し、実装していきます。
