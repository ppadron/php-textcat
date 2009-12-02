<?php

require_once "PHPUnit/Framework/TestCase.php";

class TextCategorizerTest extends PHPUnit_Framework_TestCase
{

    public function testShouldThrowExceptionIfTextIsShorterThan25Chars()
    {
        $textcat = new TextCategorizer("data/conf.txt");
        $string = "Very short string";
        $this->expectedException = "TextCategorizerException";
        $result = $textcat->classify($string);
    }

}
