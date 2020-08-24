--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list3 = array(
    "list1"=> array(
        "item1"=> 5,
        "item2"=> array(
            "subItem1"=> 10,
        )
    )
);

$list4 = array(
0 => array(
    0 => 5,
    1 => array(
        0 => 10,
    )
)
);

$content6 ='
{var:list1[item1]}
{var:list1[item2][subItem1]}';

$content7 ='
{var:0[0]}
{var:0[1][0]}';

echo BQen_Render($content6, $list3). "\n";
echo BQen_Render($content7, $list4). "\n";

?>
--EXPECTF--

5
10

5
10
