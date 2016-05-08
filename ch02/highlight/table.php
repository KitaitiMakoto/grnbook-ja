<?php
namespace PDFSearch;

class Table
{
    const INSERT =
        "INSERT INTO `pdfs` (file, title, content)
         VALUES(:file, :title, :content);";
    const SELECT = "SELECT * FROM `pdfs` ORDER BY `id`;";
    const SEARCH = <<<EOS
SELECT file, title, mroonga_snippet_html(content, :query) AS snippets
FROM `pdfs` WHERE MATCH(content) AGAINST(:query IN BOOLEAN MODE);
EOS;

    protected $pdo;

    public function __construct($dsn, $username, $password)
    {
        $this->pdo = new \PDO($dsn, $username, $password);
    }

    public function insert(Upload $upload)
    {
        $params = [
            ':file'    => $upload->getName(),
            ':title'   => $upload->getTitle(),
            ':content' => $upload->getContent()
        ];
        $sth = $this->pdo->prepare(self::INSERT);
        $isSucceeded = $sth->execute($params);
        if (! $isSucceeded) {
            throw new \RuntimeException(implode(PHP_EOL, $sth->errorInfo()));
        }
    }

    public function records()
    {
        $sth = $this->pdo->prepare(self::SELECT);
        $isSucceeded = $sth->execute();
        if (! $isSucceeded) {
            throw new \RuntimeException(implode(PHP_EOL, $sth->errorInfo()));
        }
        return $sth->fetchAll();
    }

    public function search($query)
    {
        $sth = $this->pdo->prepare(self::SEARCH);
        $params = [':query' => $query];
        $isSucceeded = $sth->execute($params);
        if (! $isSucceeded) {
            throw new \RuntimeException(implode(PHP_EOL, $sth->errorInfo()));
        }
        return $sth->fetchAll();
    }
}
