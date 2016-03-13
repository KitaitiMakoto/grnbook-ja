<?php
define('DBNAME', 'pdfsearch');

$dbh = new PDO('mysql:host=localhost;charset=utf8', 'root', '');

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $dbh = new PDO('mysql:host=localhost;charset=utf8', 'root', '');
    $result = $dbh->exec('CREATE DATABASE ' . DBNAME);

    header("Location: {$_SERVER['REQUEST_URI']}");
}

$result = $dbh->query('SHOW DATABASES')->fetchAll();
$databases = array_map(function($row) {
    return $row['Database'];
}, $result);
?>
<!doctype html>
<html>
  <head>
    <title>データベースの作成</title>
  </head>
  <body>
    <h1>データベースの作成</h1>
    <?php if (! (in_array(DBNAME, $databases))): ?>
    <form method="post">
      <input type="submit" value="pdfsearchデータベースの作成">
    </form>
    <?php endif; ?>
    <h2>現在のデータベース一覧</h2>
    <ul>
    <?php foreach ($databases as $database): ?>
      <li><?php echo htmlspecialchars($database, ENT_QUOTES, 'UTF-8'); ?></li>
    <?php endforeach; ?>
    </ul>
  </body>
</html>
