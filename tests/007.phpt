--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list5 = array(
    "list1"=> array(
        "item1"=> array(
            "subItem1"=> "1",
            "subItem2"=> "three",
            "subItem3"=> true,
            "subItem4"=> null,
        ),
        "item2"=> array(
            "subItem1"=> 1,
            "subItem2"=> 0,
            "subItem3"=> 5,
            "subItem4"=> 7,
        )
    )
);

$content8 = '<loop set="list1[item1]" value="val">
{var:val}</loop>';

$content9 = '<loop set="list1[item2]" value="val">
{var:val}</loop>';

echo BQen_Render($content8, $list5). "\n";
echo BQen_Render($content9, $list5). "\n";

?>
--EXPECTF--

1
three
true
null

1
0
5
7
