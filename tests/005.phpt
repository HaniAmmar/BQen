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

$content5 = '<loop key="[id]" value="[val]">
[id] [val]</loop>';

echo BQen_Render($content5, $list1). "\n";
echo BQen_Render($content5, $list2). "\n";

?>
--EXPECTF--

v1 null
v2 false
v3 true
v4 13.8
v5 105
v6 Qen

0 Qen
1 105
2 13.8
3 true
4 false
5 null
