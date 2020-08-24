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

$content1 = '
{var:v1}
{var:v2}
{var:v3}
{var:v4}
{var:v5}
{var:v6}';

echo BQen_Render($content1, $list1) . "\n";

?>
--EXPECTF--

null
false
true
13.8
105
Qen
