<?php
// #@@range_begin(head)
namespace PDFSearch;

class Table
{
// #@@range_end(head)
    const INSERT =
        "INSERT INTO `pdfs` (file, title, content)
         VALUES(:file, :title, :content);";
    const SELECT = "SELECT * FROM `pdfs` ORDER BY `id`;";
// #@@range_begin(sql)
    const SEARCH = <<<EOS
SELECT * FROM `pdfs` WHERE MATCH(title, content) AGAINST(:query_with_pragma IN BOOLEAN MODE);
EOS;
// #@@range_end(sql)

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

    // #@@range_begin(search)
    public function search($query)
    {
        $sth = $this->pdo->prepare(self::SEARCH);
        $params = [':query_with_pragma' => '*D+' . $query];
    // #@@range_end(search)
        $isSucceeded = $sth->execute($params);
        if (! $isSucceeded) {
            throw new \RuntimeException(implode(PHP_EOL, $sth->errorInfo()));
        }
        return $sth->fetchAll();
    }
}
