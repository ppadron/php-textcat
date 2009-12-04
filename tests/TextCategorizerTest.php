<?php

require_once "PHPUnit/Framework/TestCase.php";

class TextCategorizerTest extends PHPUnit_Framework_TestCase
{
/*
    public function setUp()
    {
        chdir(dirname(__FILE__));
        $this->textcat = new TextCategorizer("data/conf.txt");
    }
*/
    public function testShouldAcceptConfAsArray()
    {
        $config = array(
            "portuguese" => dirname(__FILE__) . "/data/LM/portuguese.lm",
            "english"    => dirname(__FILE__) . "/data/LM/english.lm",
        );

        $this->textcat = new TextCategorizer($config);
        $this->assertEquals(
            array("english"),
            $this->textcat->classify("This text should be classified as English and only English, no other language")
        );
    }
/*
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
*/

}
