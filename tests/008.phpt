--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list1 = array(0,1);
$list2 = array('a'=>'A');

$content1 = '
{var:5}
{var:100}
{var:W}
{var:T}';

echo BQen_Render($content1, $list1) . "\n";
echo BQen_Render($content1, $list2) . "\n";

?>
--EXPECTF--

{var:5}
{var:100}
{var:W}
{var:T}

{var:5}
{var:100}
{var:W}
{var:T}
