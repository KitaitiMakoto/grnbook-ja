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
    }

    public function getName()
    {
        return $this->name;
    }
}
