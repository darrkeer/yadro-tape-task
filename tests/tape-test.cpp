#include <gtest/gtest.h>

#include "../src/tape.h"
#include "../src/tape-algo.h"

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

void check_state(const std::vector<int> &v, tape &t) {
    std::size_t prev_i = t.head();
    while (t.head() > 0) {
        t.lshift();
    }
    for (int i = 0; i < v.size(); ++i) {
        ASSERT_EQ(v[i], t.read());
        if (i != v.size() - 1) {
            t.rshift();
        }
    }
    while (t.head() > prev_i) {
        t.lshift();
    }
}

void check_state(std::vector<std::pair<std::string, std::string> > v, std::map<std::string, std::string> mp) {
    int i = 0;
    for (auto &[key, value]: mp) {
        ASSERT_EQ(std::make_pair(key, value), v[i++]);
    }
}

void load(tape &t, const std::vector<int> &data) {
    ASSERT_EQ(t.capacity(), data.size());
    tape_algo::shift(t, 0);
    t.write(data[0]);
    for (std::size_t i = 1; i < t.capacity(); ++i) {
        t.rshift();
        t.write(data[i]);
    }
}

TEST(base, small_buffer) {
    tape t(2, 1);
    t.write(1);
    ASSERT_EQ(t.read(), 1);
    t.write(2);
    t.rshift();
    t.write(3);
    ASSERT_EQ(t.read(), 3);
    t.lshift();
    ASSERT_EQ(t.read(), 2);
    ASSERT_EQ(t.read(), 2);

    check_state({2, 3}, t);
}

TEST(base, capacity_is_not_multiple_for_buffer_capacity) {
    int N = 30;
    int M = 7;
    tape t(N, M);
    std::vector<int> v(N);
    for (int i = 0; i < N; ++i) {
        t.write(i);
        v[i] = i;
        if (i != N - 1) {
            t.rshift();
        }
    }
    for (int i = 0; i < N; ++i) {
        t.write(0);
        ASSERT_EQ(t.read(), 0);
        if (i != N - 1) {
            t.lshift();
        }
    }
    for (int i = 0; i < N; ++i) {
        t.write(i);
        if (i != N - 1) {
            t.rshift();
        }
    }
    check_state(v, t);
}

TEST(base, capacity_is_less_than_buffer) {
    tape t(2, 5);
    load(t, {1, 2});
    check_state({1, 2}, t);
    load(t, {3, 4});
    check_state({3, 4}, t);
}

TEST(base, random_test) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int REPEAT = 5;
    int N = 1000000;
    std::uniform_int_distribution<int> distribution(1, N);
    int M = distribution(gen);

    tape t(N, M);
    std::vector<int> v(N);
    for (int _ = 0; _ < REPEAT; ++_) {
        for (int i = 0; i < N; ++i) {
            v[i] = distribution(gen);
            t.write(v[i]);
            if (i != N - 1) {
                t.rshift();
            }
        }
        while (t.head() != 0) {
            t.lshift();
        }
        check_state(v, t);
    }
}

TEST(cfg_parser, correct) {
    cfg_parser parser("../tests/configs/1.txt");
    check_state({{"a", "123"}, {"b", "456"}}, parser.data());
}

TEST(cfg_parser, incorrect) {
    ASSERT_THROW(cfg_parser("../tests/configs/incorrect.txt"), tape_exception);
}

TEST(cfg_parser, non_existing_file) {
    ASSERT_THROW(cfg_parser("../tests/configs/noopen.txt"), tape_exception);
}

TEST(cfg, incorrect) {
    tape t(10, 10);
    ASSERT_THROW(t.set_config_file("../tests/configs/incorrect_tape_cfg.txt"), tape_exception);
}

TEST(cfg, long_delay) {
    int N = 100;
    int M = 2;
    tape t(N, M);
    t.set_config_file("../tests/configs/correct_tape_cfg.txt");
    std::vector<int> v(N);
    t.write(0);
    for (int i = 0; i < N; ++i) {
        t.write(i);
        v[i] = i;
        if (i != N - 1) {
            t.rshift();
        }
    }
    check_state(v, t);
}

TEST(merge, base) {
    tape t(10, 2);
    load(t, {1, 3, 4, 5, 7, 2, 6, 8, 9, 10});
    tape_algo::merge(t, 0, 5, 5, 10);
    check_state({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, t);
}

TEST(sort, base) {
    tape t1(10, 2);
    load(t1, {10, 9, 8, 5, 4, 10, 2, 8, 9, 9});
    tape_algo::sort(t1);
    check_state({2, 4, 5, 8, 8, 9, 9, 9, 10, 10}, t1);
}

TEST(sort, random) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int REPEAT = 5;
    int N = 100000;
    std::uniform_int_distribution<int> distribution(1, N);
    int M = distribution(gen);

    std::vector<int> v(N);

    for (int _ = 0; _ < REPEAT; ++_) {
        tape t(N, M);
        for (int i = 0; i < N; ++i) {
            v[i] = distribution(gen);
            t.write(v[i]);
            t.rshift();
        }
        std::sort(v.begin(), v.end());
        tape_algo::sort(t);
        check_state(v, t);
    }
}
