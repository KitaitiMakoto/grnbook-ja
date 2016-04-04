<?php
require_once __DIR__ . '/upload.php';

function h($string, $flags = ENT_QUOTES, $encoding = 'UTF-8')
{
    return htmlspecialchars($string, $flags, $encoding);
}

if ($_SERVER['REQUEST_METHOD'] === 'POST' && array_key_exists('pdf', $_FILES)) {
    $uploads = \PDFSearch\Upload::fromFilesInfo($_FILES['pdf']);
}

?><!doctype html>
<title>PDF Search</title>
<h1>PDF Search</h1>

<h2>アップロードされたファイル</h2>
<?php if (! empty($uploads)): ?>
<?php foreach ($uploads as $upload): ?>
<p><?php echo h($upload->getName()); ?></p>
<?php endforeach; ?>
<?php endif; ?>

<form enctype="multipart/form-data" method="post">
  <input name="pdf[]" type="file" multiple>
  <input type="submit">
</form>
