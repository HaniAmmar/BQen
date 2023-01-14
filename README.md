[![Build Status](https://travis-ci.com/HaniAmmar/BQen.svg?branch=main)](https://travis-ci.com/HaniAmmar/BQen)
[![Build status](https://ci.appveyor.com/api/projects/status/e2w6byu98k4kg804/branch/main?svg=true)](https://ci.appveyor.com/project/HaniAmmar/bqen/branch/main)

# BQen

-   [What Is BQen](#what-is-bqen)
-   [Features](#features)
-   [Usage](#usage)
    -   [Variable](#variable)
    -   [Math](#math)
    -   [Inline If](#inline-if)
    -   [Loop](#loop)
    -   [If Condition](#if-condition)
-   [Live Testing](#live-testing)
-   [Page Example](#page-example)
-   [Reference](#reference)
-   [Requirements](#requirements)
-   [Building](#building)
-   [License](#license)

## What Is BQen

A PHP module that uses [Qentem](https://github.com/HaniAmmar/Qentem-Engine) library for fast and secure template rendering.

## Features

-   Fast template rendering.
-   Low memory usage.
-   Safe evaluation.
-   Variable replacement with HTML auto-escape.
-   Raw variable replacement **without** HTML auto-escape.
-   Nested loop (with data grouping and sorting).
-   Nested if condition.
-   Inline if.
-   Math tag.

## Usage

Templates' content should be stored in a string variable, and the substituting values in an array or a JSON string, then pass them to **BQen_Render(string, array|string, string?)** and it will return a string containing the final result.

### Variable

```php
$list = array(
    'v1' => 'Qentem',
    'sub-list1' => array('sv1' => 'BQen'),
    'sub-list2' => array(77)
);

$template = '
{var:v1}
{var:sub-list1[sv1]}
{var:sub-list2[0]}
';

echo BQen_Render($template, $list);
```

### Math

```php
$list = array(
    'Equation' => '1+4*2+1',
    'one'      => '1',
    'three'    => '3'
);

$template = '
0.1+0.2 is: {math: 0.1  +   0.2 }
{var:Equation} = {math:{var:Equation}}; (1+8+1)
6^2 = {math:6^2}
--1 = {math:--1}
{var:one}+{var:three} = {math:{var:one}+{var:three}}
9 % 5 = {math:9 % 5}
';

echo BQen_Render($template, $list);
```

### Inline If

```php
$list = array(
    'one'      => '1',
    'two'      => '2',
    'three'    => '3',
    'name'     => 'Qentem',
);

$template = '
{if case="{var:one} + {var:two} >= {var:three}" true="3" false="not three"}
{if case="{var:one}" true="{var:one}" false="not one"}
{if case="{var:name} == Qentem" true="Qentem!"}
';

echo BQen_Render($template, $list);
```

### Loop

```php
$list = array(
    'object' =>
            array (
                array("var1" => "value1", "var2" => "value2","var3" => "value3", "var4" => "value4"),
                array("var1" => "value5", "var2" => "value6","var3" => "value7", "var4" => "value8")
            ),
    'array'  =>
            array (
                array("value10", "value20", "value30", "value40"),
                array("value100", "value200", "value300", "value400")
            )
);


$template = '
<loop set="object" value="item">
{var:item[var1]} {var:item[var2]} {var:item[var3]} {var:item[var4]}</loop>

<loop set="array" value="item">
{var:item[0]} {var:item[1]} {var:item[2]} {var:item[3]}</loop>
';

echo BQen_Render($template, $list);

// To cache the template for multi-rendering, and a name:
// echo BQen_Render($template, $list, 'template_1');
```

### If Condition

```php
$list = array(0,1,2,3);


$template = '
<if case="{var:0} == 0">
Zero!
</if>

<if case="{var:1} == 0">
Zero!
<else />
Not {var:0} but {var:1}.
</if>

<if case="{var:2} == 0">
Zero!
<elseif case="{var:2} == 2" />
Two!
<else />
Not zero or one.
</if>

<if case="{var:2} == 0">
Zero!
<elseif case="{var:2} == 5" />
Two!
<elseif case="{var:3} == 3" />
{var:3}
<else />
Not zero or one or two.
</if>
';

echo BQen_Render($template, $list);

// To cache the template for multi-rendering, and a name:
// echo BQen_Render($template, $list, 'template_2');
```

## Live Testing

Templates can be tested live @ [JQen Tool](https://haniammar.github.io/JQen-Tool)

## Page Example

```php
$json_string = '
[
    {
        "major": "Computer Science",
        "students": [
            { "Name": "Oliver", "GPA": 3.2 },
            { "Name": "Jonah", "GPA": 3.8 },
            { "Name": "Jack", "GPA": 2.8 }
        ]
    },
    {
        "major": "Math",
        "students": [
            { "Name": "Maxim", "GPA": 3.0 },
            { "Name": "Cole", "GPA": 2.5 },
            { "Name": "Claire", "GPA": 2.4 }
        ]
    }
]
';


$template = '
<html>

<body>
    <div>
        <h2>Students\' list:</h2>
        <loop value="department_val">
            <h3>Major: {var:department_val[major]}</h3>
            <ul>
            <loop set="department_val[students]" value="student_val">
                <li>
                    <span>Name: {var:student_val[Name]}</span>
                    <span>
                        GPA: {var:student_val[GPA]}
                        <if case="{var:student_val[GPA]} < 2.5"> (Inform adviser!)
                        <elseif case="{var:student_val[GPA]} >= 3.5" /> (President\'s List!)
                        <elseif case="{var:student_val[GPA]} >= 3.0" /> (Dean\'s List!)
                        </if>
                    </span>
                </li>
            </loop>
            </ul>
        </loop>
    </div>
</body>

</html>
';

echo BQen_Render($template, json_decode($json_string, true));

// To cache the template for multi-rendering, and a name:
// echo BQen_Render($template, $list, 'mainpage');
```

## Reference

Syntax @ [Qentem-Engine/Template.md](https://github.com/HaniAmmar/Qentem-Engine/blob/main/Documentation/Template.md).

## Requirements

-   C++ 11+ compiler
-   PHP 7+

## Building

### Linux & MacOS

```sh
git submodule update --init

phpize
./configure
make
sudo make install
```

After that, add `extension=bqen.so` to php.ini or place it in its own file in `php/conf.d`.

## License

> MIT License
>
> Copyright (c) 2020 Hani Ammar
>
> Permission is hereby granted, free of charge, to any person obtaining a copy
> of this software and associated documentation files (the "Software"), to deal
> in the Software without restriction, including without limitation the rights
> to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
> copies of the Software, and to permit persons to whom the Software is
> furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all
> copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
> IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
> FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
> AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
> LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
> OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
> SOFTWARE.
