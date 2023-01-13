--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list1 = array(
    'v1' =>  null,
    'v2' =>  false,
    'v3' =>  true,
    'v4' =>  13.8,
    'v5' =>  105,
    'v6' =>  'Qen',
);

$list2 = array(
    0 =>  'Qen',
    1 =>  105,
    2 =>  13.8,
    3 =>  true,
    4 =>  false,
    5 =>  null,
);

$content4 = '<loop value="val">
{var:val}</loop>';

echo BQen_Render($content4, $list1). "\n";
echo BQen_Render($content4, $list2). "\n";

?>
--EXPECTF--

null
false
true
13.8
105
Qen

Qen
105
13.8
true
false
null
