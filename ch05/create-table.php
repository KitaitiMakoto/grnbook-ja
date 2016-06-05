<?php
define('DBNAME', 'pdfsearch');
define('TABLE_NAME', 'pdfs');

$dsn = 'mysql:host=localhost;dbname=' . DBNAME . ';charset=utf8';
$dbh = new PDO($dsn, 'root', '');
$table = TABLE_NAME;

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $statement = <<<STATEMENT
CREATE TABLE `{$table}` (
    id      INT PRIMARY KEY AUTO_INCREMENT,
    file    VARCHAR(255),
    title   VARCHAR(255),
    content LONGTEXT,
    FULLTEXT INDEX (title,content)
) ENGINE = Mroonga DEFAULT CHARSET utf8;
STATEMENT;
    $dbh->exec($statement);

    header("Location: {$_SERVER['REQUEST_URI']}");
}

$result = $dbh->query('SHOW TABLES')->fetchAll();
$tables = array_map(function($row) {
    return $row[0];
}, $result);

?>
<!doctype html>
<html>
  <head>
    <title>テーブルの作成</title>
  </head>
  <body>
    <h1>テーブルの作成</h1>
    <?php if (! (in_array(TABLE_NAME, $tables))): ?>
    <form method="post">
      <input type="submit" value="pdfsテーブルの作成">
    </form>
    <?php endif; ?>
    <h2>現在のテーブル一覧</h2>
    <ul>
    <?php foreach ($tables as $table): ?>
      <li><?php echo htmlspecialchars($table, ENT_QUOTES, 'UTF-8') ?></li>
    <?php endforeach; ?>
    </ul>
  </body>
</html>
