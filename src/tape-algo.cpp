#include "tape-algo.h"

tape tape_algo::copy(tape& t) {
    return copy(t, 0, t.capacity());
}

tape tape_algo::copy(tape& t, const std::size_t from, const std::size_t to) {
    const std::size_t prev_head = t.head();
    shift(t, from);
    tape res(to - from, t.buffer_capacity());
    while (t.head() != to) {
        res.write(t.read());
        t.rshift();
        res.rshift();
    }
    shift(t, prev_head);
    return res;
}

void tape_algo::shift(tape& t, const std::size_t i) {
    while (t.head() < i) {
        t.rshift();
    }
    while (t.head() > i) {
        t.lshift();
    }
}

void tape_algo::ensure_capacity(tape& t, std::size_t capacity) {
    while (t.capacity() - t.head() < capacity) {
        t.lshift();
    }
}

void tape_algo::load(tape& in, tape& out, std::size_t len) {
    ensure_capacity(out, len);
    std::size_t start_in = in.head();
    std::size_t start_out = out.head();
    for (std::size_t i=0; i<len; ++i) {
        out.write(in.read());
        in.rshift();
        out.rshift();
    }
    shift(in, start_in);
    shift(out, start_out);
}

void tape_algo::merge(tape& t, tape& tmp1, tape& tmp2, std::size_t l1, std::size_t r1, std::size_t l2, std::size_t r2) {
    shift(t, l1);

    const std::size_t len_l = r1 - l1;
    const std::size_t len_r = r2 - l2;
    load(t, tmp1, len_l);
    shift(t, l2);
    load(t, tmp2, len_r);
    shift(t, l1);
    const std::size_t tmp1_r = tmp1.head() + len_l;
    const std::size_t tmp2_r = tmp2.head() + len_r;
    for (std::size_t i=0; i<len_l + len_r; ++i) {
        if (tmp2.head() == tmp2_r || tmp1.head() != tmp1_r && tmp1.read() < tmp2.read()) {
            t.write(tmp1.read());
            tmp1.rshift();
        } else {
            t.write(tmp2.read());
            tmp2.rshift();
        }
        t.rshift();
    }
}

void tape_algo::sort(tape& t, tape& tmp1, tape& tmp2, std::size_t l, std::size_t r) {
    if (l + 1 == r) {
        return;
    }
    shift(t, l);

    std::size_t m = (l + r) / 2;
    sort(t, tmp1, tmp2, l, m);
    sort(t, tmp1, tmp2, m, r);
    merge(t, tmp1, tmp2, l, m, m, r);
}

void tape_algo::merge(tape &t, std::size_t l1, std::size_t r1, std::size_t l2, std::size_t r2) {
    tape tmp1 = copy(t);
    tape tmp2 = copy(t);
    merge(t, tmp1, tmp2, l1, r1, l2, r2);
}

void tape_algo::sort(tape &t) {
    sort(t, 0, t.capacity());
}

void tape_algo::sort(tape &t, std::size_t l, std::size_t r) {
    tape tmp1 = copy(t);
    tape tmp2 = copy(t);
    sort(t, tmp1, tmp2, l, r);
}


