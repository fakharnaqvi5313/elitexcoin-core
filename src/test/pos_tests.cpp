// Copyright (c) 2026 The EliteXcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <consensus/amount.h>
#include <pos.h>

#include <test/util/setup_common.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(pos_tests, BasicTestingSetup)

static const int64_t SECONDS_PER_AVG_MONTH = 2629800; // 365.25d / 12

// EliteXcoin pays 1.72% of a stake's value per average month of coin-age,
// pro-rated by the second, uncapped. These tests pin the exact formula
// (see GetProofOfStakeReward() in pos.h/pos.cpp) at values chosen so the
// integer division is exact, so there's no ambiguity from floor rounding.

BOOST_AUTO_TEST_CASE(pos_reward_zero_inputs)
{
    BOOST_CHECK_EQUAL(GetProofOfStakeReward(0, SECONDS_PER_AVG_MONTH), 0);
    BOOST_CHECK_EQUAL(GetProofOfStakeReward(100 * COIN, 0), 0);
    BOOST_CHECK_EQUAL(GetProofOfStakeReward(-100 * COIN, SECONDS_PER_AVG_MONTH), 0);
    BOOST_CHECK_EQUAL(GetProofOfStakeReward(100 * COIN, -1), 0);
}

BOOST_AUTO_TEST_CASE(pos_reward_one_month_is_1_72_percent)
{
    // 10,000 EXC staked for exactly one average month -> exactly 1.72% = 172 EXC
    CAmount stake = 10000 * COIN;
    CAmount reward = GetProofOfStakeReward(stake, SECONDS_PER_AVG_MONTH);
    BOOST_CHECK_EQUAL(reward, 172 * COIN);
}

BOOST_AUTO_TEST_CASE(pos_reward_scales_linearly_with_time)
{
    CAmount stake = 10000 * COIN;
    // Half a month -> half the reward (2629800 is even, so this divides exactly)
    CAmount halfMonthReward = GetProofOfStakeReward(stake, SECONDS_PER_AVG_MONTH / 2);
    BOOST_CHECK_EQUAL(halfMonthReward, 86 * COIN);

    // Two months -> double the reward
    CAmount twoMonthReward = GetProofOfStakeReward(stake, SECONDS_PER_AVG_MONTH * 2);
    BOOST_CHECK_EQUAL(twoMonthReward, 344 * COIN);
}

BOOST_AUTO_TEST_CASE(pos_reward_scales_linearly_with_stake_size)
{
    CAmount small = 1000 * COIN;
    CAmount large = 100000 * COIN; // 100x
    CAmount smallReward = GetProofOfStakeReward(small, SECONDS_PER_AVG_MONTH);
    CAmount largeReward = GetProofOfStakeReward(large, SECONDS_PER_AVG_MONTH);
    BOOST_CHECK_EQUAL(smallReward, small * 172 / 10000); // 17.2 EXC, exact
    BOOST_CHECK_EQUAL(largeReward, smallReward * 100);
}

BOOST_AUTO_TEST_CASE(pos_reward_full_premine_scale_no_overflow)
{
    // The entire 14B EXC premine staked continuously for one month: this is the
    // largest realistic stake the network will ever see, so it doubles as an
    // overflow-safety check for the 256-bit intermediate math.
    CAmount fullPremine = CAmount(14000000000LL) * COIN;
    CAmount reward = GetProofOfStakeReward(fullPremine, SECONDS_PER_AVG_MONTH);
    BOOST_CHECK_EQUAL(reward, CAmount(240800000LL) * COIN); // 1.72% of 14B = 240.8M
    BOOST_CHECK(MoneyRange(reward));
}

BOOST_AUTO_TEST_CASE(pos_reward_uncapped_coin_age)
{
    // The spec calls for "no maximum age" on stake eligibility. Because reward
    // accrual uses that same uncapped coin-age, a balance left unstaked for
    // years earns proportionally more in a single stake event - e.g. 5 years
    // (60 months) of coin-age pays out 60 * 1.72% = 103.2% of the stake in one
    // block. This is intentional per the coin spec, not a bug; documented here
    // so the consequence is explicit and covered by a test.
    CAmount stake = 1000 * COIN;
    CAmount fiveYearReward = GetProofOfStakeReward(stake, SECONDS_PER_AVG_MONTH * 60);
    BOOST_CHECK_EQUAL(fiveYearReward, 1032 * COIN); // 103.2% of 1000
    BOOST_CHECK(fiveYearReward > stake);
}

BOOST_AUTO_TEST_SUITE_END()
