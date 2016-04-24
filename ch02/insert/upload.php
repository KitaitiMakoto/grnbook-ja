<?php
namespace PDFSearch;

class Upload
{
    protected $name;
    protected $tmpName;

    public static function fromFilesInfo(array $info)
    {
        $uploads = array();
        $fileCount = count($info['name']);
        for ($i = 0; $i < $fileCount; $i++) {
            $name = $info['name'][$i];
            $tmpName = $info['tmp_name'][$i];
            $uploads[] = new Upload($name, $tmpName);
        }
        return $uploads;
    }

    public function __construct($name, $tmpName)
    {
        $this->name = $name;
        $this->tmpName = $tmpName;
        $this->pdf = new \Poppler\Processor\PdfFile(
          \Poppler\Driver\Pdfinfo::create(),
          \Poppler\Driver\Pdftotext::create(),
          \Poppler\Driver\Pdftohtml::create()
        );
    }

    public function getName()
    {
        return $this->name;
    }

    public function getTitle()
    {
        $info = $this->pdf->getInfo($this->tmpName);
        return $info['Title'];
    }

    public function getContent()
    {
        return $this->pdf->toText($this->tmpName);
    }
}
