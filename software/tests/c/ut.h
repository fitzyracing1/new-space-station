/* SPDX-License-Identifier: MIT  - minimal unit-test helpers */
#ifndef UT_H
#define UT_H
#include <stdio.h>
#include <math.h>
static int ut_fail = 0, ut_pass = 0;
#define CHECK(c) do { if (c) ut_pass++; else { ut_fail++; printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #c); } } while (0)
#define CHECK_NEAR(a, b, tol) CHECK(fabs((double)(a) - (double)(b)) <= (tol))
#define UT_DONE(name) do { printf("%s: %d passed, %d failed\n", name, ut_pass, ut_fail); return ut_fail ? 1 : 0; } while (0)
#endif
