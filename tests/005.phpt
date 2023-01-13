--TEST--
BQen_Render() tests
--SKIPIF--
<?php if (!extension_loaded("bqen")) print "skip"; ?>
--FILE--
<?php

$list1 = '{"numbers":[1,2,3,4,5,6,7,8],"qen":"Qentem"}';

$content1 = '
<loop set="numbers" value="this_number">
<if case="({var:this_number} % 2) == 1">{var:this_number} is an odd number.</if></loop>

((1+2)^3)/2 = {math: ((1+2)^3)/2}

(0.1 + 0.2) is {math:   0.1   +   0.2  }

{if case="{var:qen} == Qentem" true="{var:qen}"} Engine

{if case="{var:numbers[0]} == {var:numbers[4]}" false="it\'s not {var:numbers[4]}!"}
';

echo BQen_Render($content1, $list1) . "\n";

?>
--EXPECTF--

1 is an odd number.

3 is an odd number.

5 is an odd number.

7 is an odd number.


((1+2)^3)/2 = 13.5

(0.1 + 0.2) is 0.3

Qentem Engine

it's not 5!
