//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// test operator new[] replacement by replacing only operator new

// UNSUPPORTED: sanitizer-new-delete
// XFAIL: libcpp-no-vcruntime


#include <new>
#include <cstddef>
#include <cstdlib>
#include <cassert>
#include <limits>

#include "count_new.hpp"
#include "test_macros.h"

int A_constructed = 0;

struct A
{
    A() {++A_constructed;}
    ~A() {--A_constructed;}
};

int main()
{
    globalMemCounter.reset();
    assert(globalMemCounter.checkOutstandingNewEq(0));
    A *ap = new A[3];
    DoNotOptimize(ap);
    assert(ap);
    assert(A_constructed == 3);
    assert(globalMemCounter.checkOutstandingNewEq(1));
    delete [] ap;
    DoNotOptimize(ap);
    assert(A_constructed == 0);
    assert(globalMemCounter.checkOutstandingNewEq(0));
}
