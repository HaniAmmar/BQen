PHP_ARG_ENABLE(BQen, Whether to enable BQen module, [  --enable-bqen           Enable BQen module])

if test "$PHP_BQEN" != "no"; then
    CXXFLAGS="-std=c++11 -O3 -march=native -Wall -Wextra -fno-exceptions"
    PHP_REQUIRE_CXX()
    AC_DEFINE(HAVE_BQEN, 1, [Have BQen module])
    PHP_NEW_EXTENSION(bqen, bqen.cpp, $ext_shared)
    PHP_ADD_INCLUDE([$ext_srcdir/qentem/Include])
fi
