= データベースの作成

まず、PHPを使って、MySQLにデータベースを作成します。作成の手順は通常のMySQLでのデータベース作成と同様です。Dockerコンテナ内では既にMySQLが動作しており、@<code>{root}ユーザーでパスワード無しでログインできます。

ソースコードは以下のようになります。@<fn>{loosephp}

//list[ch04/create-database.php][create-database.php][php]{
#@mapfile(ch04/create-database.php)
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
