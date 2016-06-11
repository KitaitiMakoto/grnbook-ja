<?php
namespace PDFSearch;

class Upload
{
    protected $name;
    protected $tmpName;

    public static function fromFilesInfo(array $info)
    {
        $uploads = array();
        foreach ($info['name'] as $number => $name) {
            $tmpName = $info['tmp_name'][$number];
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
