--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list2 = array(
    0 =>  'Qen',
    1 =>  105,
    2 =>  13.8,
    3 =>  true,
    4 =>  false,
    5 =>  null,
);

$content2 = '
{var:0}
{var:1}
{var:2}
{var:3}
{var:4}
{var:5}';

echo BQen_Render($content2, $list2) . "\n";

?>
--EXPECTF--

Qen
105
13.8
true
false
null
