<?php
// Dockerイメージ内にComposerでインストール済みのライブラリーを読み込む
require_once 'vendor/autoload.php';
require_once __DIR__ . '/upload.php';
require_once __DIR__ . '/table.php';

define('DBNAME', 'pdfsearch');

$dsn = 'mysql:host=localhost;dbname=' . DBNAME . ';charset=utf8';
$table = new \PDFSearch\Table($dsn, 'root', '');

function h($string, $flags = ENT_QUOTES, $encoding = 'UTF-8')
{
    return htmlspecialchars($string, $flags, $encoding);
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && array_key_exists('pdf', $_FILES)) {
    $uploads = \PDFSearch\Upload::fromFilesInfo($_FILES['pdf']);
    try {
        foreach ($uploads as $upload) {
            $tableb->insert($upload);
        }
        header('Location: .');
    } catch (\RuntimeException $e) {
        header('HTTP', true, 500);
        header('content-type: text/plain');
        echo $e;
    }
    exit;
}

$records = $table->records();

?><!doctype html>
<title>PDF Search</title>
<h1>PDF Search</h1>

<h2>登録済みPDF一覧</h2>
<table>
  <tr>
    <th>ファイル名</th>
    <th>タイトル</th>
    <th>内容</th>
  </tr>
  <?php foreach ($records as $record): ?>
    <tr>
      <td><?php echo h($record['file']); ?></td>
      <td><?php echo h($record['title']); ?></td>
      <td><?php echo h(mb_substr($record['content'], 0, 120)); ?>&hellip;&hellip;</td>
    </tr>
  <?php endforeach; ?>
</table>

<form enctype="multipart/form-data" method="post">
  <input name="pdf[]" type="file" multiple>
  <input type="submit">
</form>
