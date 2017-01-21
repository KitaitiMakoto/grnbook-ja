= Mroongaならではの機能の作成

ここからは、MySQLの全文検索機能にはない、Mroongaならではの特徴を活かした機能を実装していきます。

== 検索語のハイライト

現在の検索機能では、検索結果中、どこに検索語があるのか、自分で探さなくてはなりません。次の画像のように、検索語だけがハイライトされていると、とても見やすくなります。また、菌類についてのPDFを探すつもりで「きのこ」で検索し「お気づきのこと」といった関係ない語がヒットした場合に、すぐにそのことに気付けるというメリットもあります。

//image[highlight][検索語をハイライトして表示]{
//}

このように検索語をハイライトする機能を実装してみましょう。

すぐに思い付く実装方法は、取得した検索結果の@<code>{content}カラムを、再度PHPで解析してタグを挿入することですが、実はMroongaには、このための機能が備わっています。SQL中で利用可能な@<code>{mroonga_snippet_html()}関数です。これは特定のカラム（ここでは@<code>{content}）のうち、指定した語（「cat」）の前後数十バイトを抜き出し、文字列として返す関数です。

@<code>{mroonga_snippet_html()}関数が返すスニペットは@<code>{<div class="snippet">}...@<code>{</div>}というタグで囲まれ、更に検索語は@<code>{<span class="keyword">}...@<code>{</span>}で囲まれます@<fn>{mroonga_snippet}。一レコード中に検索語が複数回現れる場合は、その数に応じて複数のスニペットを結合した一つの文字列を返します。余談ですが「スニペット」は「断片」という意味で、この場合は@<code>{content}カラムのうちの一部を返すことを意味しています。

@<code>{mroonga_snippet_html()}は以下のようにして使用します。

//emlist[mroonga_snippet_htmlの簡単な使い方][SQL]{
SELECT mroonga_snippet_html(content, 'きのこ') FROM `pdfs`;
//}

結果は、例えば次のようになります@<fn>{linebreak}。

//emlist[mroonga_snippet_html()の結果例][HTML]{
<div class="snippet">ットとするつもりだったのに間違えて git add * と打ち込んでし
まったと<span class="keyword">きのこ</span>とを考えましょう。 ファイルが両方ともステージされて
しまいました</div><div class="snippet">細は、“何が変わるのかの把握” を参照ください
もうひとつお気づ<span class="keyword">きのこ</span>とがあることでしょう。 GitHub は、このプル
リクエストが問題なくマー</div>
//}

ここでは、本書のアプリケーションに必要なことのみ説明しているので、詳細については公式ドキュメント（@<href>{http://mroonga.org/ja/docs/reference/udf/mroonga_snippet_html.html, 5.5.5. mroonga_snippet_html()}）を参照してください。

現在の実装では検索でヒットした時に@<code>{content}カラムの抜粋を表示していますが、@<code>{mroonga_snippet_html()}の戻り値で置き換えてみましょう。@<code>{PDFSearch\Table::search()}で使用しているSQL（@<code>{PDFSearch\Table::SEARCH}定数）を変更します。@<code>{mroonga_snippet_html()}も@<code>{COUNT()}などと同様の関数なので、@<code>{AS}句を使って別名を付けることができます。

//list[ch06/highlight/table.php][table.php][php]{
#@mapfile(ch06/highlight/table.php)
#@end
//}

HTML中で@<code>{content}カラムを表示していた所を、@<code>{snippets}に置き換えます。

//list[ch06/highlight/index.php][index.php][php]{
#@mapfile(ch06/highlight/index.php)
#@end
//}

@<code>{snippets}の表示ではHTMLエスケープをしていないことに気が付いたでしょうか。@<code>{mroonga_snippet_html()}にHTMLタグが含まれているため、ここでHTMLエスケープしてしまうと、タグとしての役を果たさなくなってしまいます。そのため、直接出力しています。

ここにはセキュリティ上の懸念があります@<fn>{notsecurityissue}。@<code>{mroonga_snippet_html()}の戻り値では、カラム中のHTMLタグなどは全てエスケープされますが、取得したスニペットと他の文字列をPHPで結合すると、セキュリティホールになり得ます。加工せず、可能であれば単独で出力するようにしてください。加工や他の文字列との結合が必要な場合は、慎重に行ってください。

これで、検索結果のハイライトも出来ました。実際に検索して結果を楽しみましょう。

//footnote[mroonga_snippet][タグや@<code>{class}属性を変更したい場合は、柔軟性の高い@<href>{http://mroonga.org/ja/docs/reference/udf/mroonga_snippet.html, mroonga_snippet()}関数を使用してください。]
//footnote[linebreak][読みやすさのため、改行を調整しています。]
//footnote[notsecurityissue][実際にはHTMLを正しく出力するために必要な注意点です。仮にセキュリティ上問題とならなくても、HTMLが正しく出力されず、アプリケーションが壊れてしまう可能性があります。]

== スコアによる並び替え

これまでは、検索結果の並び順については考慮していませんでした。やはり、検索語に近いPDFが先頭に並んでほしいものです。

この「検索語に近い」または「より望ましい」という状態を、Mroongaではスコアという形で、数値として計算することができます。「検索語がより多く含まれているPDFの方が望ましい」「検索語が本文に含まれているよりもタイトルに含まれている方がより望ましい」といった判断が可能になるのです。

Mroongaが使っているスコアの値は、@<code>{WHERE}句に指定している@<code>{MATH()}...@<code>{AGAINST()}の戻り値として取得できます。@<code>{AS}で別名を付けると使いやすいでしょう。

//emlist[スコアの取得][SQL]{
SELECT MATCH(title, content) AGAINST('cat' IN BOOLEAN MODE) as score FROM `pdfs`;
//}
//emlist{
+-------+
| score |
+-------+
|   315 |
|    10 |
|     0 |
|     0 |
|     4 |
+-------+
5 rows in set (0.47 sec)
//}

このスコアでソートすることで、より望ましい結果を上の方に表示することができます。

では、このスコアというのは何によって決められているのでしょうか。一つは、結果文書の中に含まれている検索語の数です。検索語が多く含まれている文書ほど検索する人の要求に合っていると見做して、検索上位に表示させられるわけです。

以下の用に@<code>{PDFSearch\Table::SEARCH}のSQLにちょっとした変更を加えるだけで、多くの場合、期待される検索結果になるでしょう。

//emlist[変更前の\PDFSearch\Table::SEARCH定数][SQL]{
SELECT file, title, mroonga_snippet_html(content, :query) AS snippets
FROM `pdfs` WHERE MATCH(title, content) AGAINST(:query IN BOOLEAN MODE);
//}

//emlist[変更前の\PDFSearch\Table::SEARCH定数][SQL]{
SELECT file, title, mroonga_snippet_html(content, :query) AS snippets,
MATCH(title, content) AGAINST(:query IN BOOLEAN MODE) AS score
FROM `pdfs` WHERE MATCH(title, content) AGAINST(:query IN BOOLEAN MODE)
ORDER BY score DESC;
//}

もう一つの基準は重みと呼ばれます。重みは、どのカラムをより重視するかという指標です。例えば、次のような二つのPDFが登録されているとします。

//table[two-groongas][Groongaという語を含む二つのPDF]{
タイトル	内容
--------------
@<em>{Groonga}で作る全文検索システム	@<em>{Groonga}を使って全文検索システムを作ります。
わたしのエッセイ集	今日、@<em>{Groonga}という言葉を耳にしました。不思議な言葉ですね、@<em>{Groonga}。検索してみると、「@<em>{Groonga}で学ぶ全文検索」という勉強会があるみたいなので、全文検索の何かなのでしょう。ところで、全文検索って何？
//}

Groongaのことが知りたくて検索する時に、どちらがヒットしてほしいでしょうか。始めの「Groongaで作る全文検索システム」の方ではないでしょうか。でも、「Groonga」という言葉が多く含まれているのは「わたしのエッセイ集」の方なので、今までの実装だと、こちらが上位に表示されることになってしまいます。前者を上位に持って来るために、「検索語がタイトルに含まれている方が、本文に含まれているよりも100倍くらい重要だ」という指標を導入することにしましょう。Mroongaまたは全文検索の言葉で「@<code>{title}カラムの重みを@<code>{content}カラムの100倍にする」ということになります。

//image[weight][検索語がタイトル含まれている場合スコアが大きくなる]{
//}

上の画像にある三つのPDFではどれも本文中に「すべて」という語が含まれますが、タイトルにも含まれる「mrubyのすべて」のスコアが、含まれない「APIデザインケーススタディ」「Dockerエキスパート養成読本」に比べ、非常に大きくなっていることが分かります。実際、タイトルの重視をやめると、「mrubyのすべて」と「APIデザインケーススタディ」の順位は逆転します。

このように重み付けするには、@<code>{W}プラグマを使用します。

//emlist[][SQL]{
MATCH(title,content) AGAINST('*W1:100,2:1 API')
//}

@<code>{W}が重み付けのためのプラグマ使用を指示し、その後はカラムごとの重み付けをカンマ区切りで指定します。「@<code>{1:100}」は「（@<code>{MATCH}で指定した）1カラム目の重みを100に」、「@<code>{2:1}」は「2カラム目の重みを1に」という命令になります。これにより、「検索語が1つタイトルに含まれていると、本文に100個含まれているのと同等のスコアになる」ということを実現しています（実際には100倍の差が付きさえすればいいので、2と200でも、10と1000でも構いません）。

まとめると、実装は次のようになります。

//list[ch06/sort/table.php][table.php][php]{
<?php
#@maprange(ch06/sort/table.php,head)
#@end
    // :
    // :
#@maprange(ch06/sort/table.php,search)
#@end
//}

@<code>{W}プラグマを色々な値に変えて試してみてください。本書のようなチュートリアルではなく、実際の全文検索システムに置いてもスコアリングは非常に大切なポイントです。重み付けを使いこなしたスコアリングができると、いかにも全文検索システムを作っているという実感が出て、楽しいと思います。
