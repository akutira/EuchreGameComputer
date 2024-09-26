// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Pack.h"
#include "unit_test_framework.h"



#include "Card.h"

#include <cassert>

#include <fstream>




#include <iostream>

using namespace std;

static const int PACK_SIZE = 24;


TEST(test_pack_default_ctor) {
    Pack pack;
    Card first = pack.deal_one();
    Card second = pack.deal_one();
    ASSERT_EQUAL(NINE, first.get_rank());
    ASSERT_EQUAL(SPADES, first.get_suit());
    ASSERT_EQUAL(TEN, second.get_rank());
    ASSERT_EQUAL(SPADES, second.get_suit());

}

TEST(test_pack_istream_ctor) {
  const string filename = "pack.in";
  ifstream ifs(filename);
  assert(ifs.is_open());
  Pack pack(ifs);
  Card first_card = pack.deal_one();
  Card second_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(NINE, SPADES));
  ASSERT_EQUAL(second_card, Card(TEN, SPADES));
}

TEST(test_pack_reset) {
  Pack pack;
  pack.deal_one();
  pack.deal_one();
  pack.deal_one();
  pack.reset();
  Card first_card = pack.deal_one();
  ASSERT_EQUAL(first_card, Card(NINE, SPADES));
}

TEST(test_pack_empty) {
  Pack pack;
  for (int i = 0; i < PACK_SIZE - 1; ++i) {
    pack.deal_one();
    ASSERT_FALSE(pack.empty());
  }
  pack.deal_one();
  ASSERT_TRUE(pack.empty());
}

// Add more tests here

TEST_MAIN()
