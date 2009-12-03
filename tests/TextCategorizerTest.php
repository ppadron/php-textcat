<?php

require_once "PHPUnit/Framework/TestCase.php";

class TextCategorizerTest extends PHPUnit_Framework_TestCase
{

    public function setUp()
    {
        $this->textcat = new TextCategorizer("data/conf.txt");
        chdir(dirname(__FILE__));
    }

    public function testShouldThrowExceptionIfTextIsShorterThan25Chars()
    {
        $this->textcat = new TextCategorizer("data/conf.txt");
        $string = "Very short string";
        $this->expectedException = "TextCategorizerException";
        $result = $this->textcat->classify($string);
    }

    public function testShouldReturnResultsAsArray()
    {
        $expected = array("english");
        $result = $this->textcat->classify("This text should be classified as English.");
        $this->assertEquals($expected, $result);
    }

}
