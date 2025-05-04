#include <gtest/gtest.h>

#include "../src/tape.h"

#include <map>
#include <string>
#include <vector>

void check_state(std::vector<int> v, tape& t) {
    std::size_t prev_i = t.head();
    while (t.head() > 0) {
        t.lshift();
    }
    for (int i=0; i<v.size(); ++i) {
        ASSERT_EQ(v[i], t.read());
        if (i != v.size() - 1) {
            t.rshift();
        }
    }
    while (t.head() > prev_i) {
        t.lshift();
    }
}

void check_state(std::vector<std::pair<std::string, std::string>> v, std::map<std::string, std::string> mp) {
    int i = 0;
    for (auto&[key, value] : mp) {
        ASSERT_EQ(std::make_pair(key, value), v[i++]);
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
    for (int i=0; i<N; ++i) {
        t.write(i);
        v[i] = i;
        if (i != N - 1) {
            t.rshift();
        }
    }
    for (int i=0; i<N; ++i) {
        t.write(0);
        ASSERT_EQ(t.read(), 0);
        if (i != N - 1) {
            t.lshift();
        }
    }
    for (int i=0; i<N; ++i) {
        t.write(i);
        if (i != N - 1) {
            t.rshift();
        }
    }
    check_state(v, t);
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
    ASSERT_THROW(tape(10, 10, "../tests/configs/incorrect_tape_cfg.txt"), tape_exception);
}

TEST(cfg, long_delay) {
    int N = 100;
    int M = 2;
    tape t(N, M, "../tests/configs/correct_tape_cfg.txt");
    std::vector<int> v(N);
    t.write(0);
    for (int i=0; i<N; ++i) {
        t.write(i);
        v[i] = i;
        if (i != N - 1) {
            t.rshift();
        }
    }
    check_state(v, t);
}