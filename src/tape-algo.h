#pragma once

#include "tape.h"

class tape_algo {
public:
    static void sort(tape& t);

    static void sort(tape& t, std::size_t l, std::size_t r);

    static void sort(tape& t, tape& tmp1, tape& tmp2, std::size_t l, std::size_t r);

    static void shift(tape& t, std::size_t i);

    static void merge(tape& t, std::size_t l1, std::size_t r1, std::size_t l2, std::size_t r2);

    static void merge(tape& t, tape& tmp1, tape& tmp2, std::size_t l1, std::size_t r1, std::size_t l2, std::size_t r2);

    static tape copy(tape& t, std::size_t from, std::size_t to);

    static tape copy(tape& t);

private:
    static void load(tape& in, tape& out, std::size_t len);

    static void ensure_capacity(tape& t, std::size_t capacity);
};