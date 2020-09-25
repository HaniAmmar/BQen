--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list1 = array(
    'a1' => 5,
    'a2' => true,
    'a3' => null,
    'a4' => false,
    'a5' => '10',
    'a6' => '20',
    'a7' => 6,
    'a8' => 1,
    'a9' => "1",
);

$content1 = '
{math:{var:a1}+8} = 13
{math:{var:a7}+8} = 14
{math:{var:a7}+{var:a1}} = 11
{math:{var:a5}+{var:a1}} = 15
{math:{var:a1}+{var:a5}} = 15
{math:{var:a6}+{var:a5}} = 30
{math:{var:a6}*{var:a2}} = 20
{math:{var:a6}*{var:a4}} = 0
{math:{var:a6}*{var:a7}} = 120
{math:{var:a7}+{var:a6}} = 26
{math:{var:a1}+{var:a2}} = 6
{math:{var:a8}=={var:a2}} = 1
{math:{var:a2}=={var:a8}} = 1
{math:{var:a5}!={var:a1}} = 1
{math:{var:a2}!={var:a4}} = 1
{math:({var:a2})==({var:a9})} = 1
{math:{var:a9}=={var:a8}} = 1
{math:{var:a8}=={var:a9}} = 1
{math:1=={var:a8}} = 1
{math:6-5==({var:a9})} = 1
{math:6-5==({var:a8})} = 1
';

echo BQen_Render($content1, $list1) . "\n";

?>
--EXPECTF--

13 = 13
14 = 14
11 = 11
15 = 15
15 = 15
30 = 30
20 = 20
0 = 0
120 = 120
26 = 26
6 = 6
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
1 = 1
