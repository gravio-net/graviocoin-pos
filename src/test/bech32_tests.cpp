// Copyright (c) 2017 Pieter Wuille
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <key_io.h>
#include <bech32.h>
#include <test/util/setup_common.h>
#include <test/util/str.h>

#include <boost/test/unit_test.hpp>

BOOST_FIXTURE_TEST_SUITE(bech32_tests, GraviocoinBasicTestingSetup)

std::vector<std::pair<std::string, std::string> > testsPass = {
    std::make_pair("GKYgnLKnvevLeUyeYgm2DnjT1FGsgVNzYE", "gh1z2kuclaye2ktkndy7mdpw3zk0nck78a7ftag2u"),
    std::make_pair("RJAPhgckEgRGVPZa9WoGSWW24spskSfLTQ", "gr1v9h7c4k0e3axk7jlejzyh8tnc5eryx6eq46g39"),
    std::make_pair("SPGxiYZ1Q5dhAJxJNMk56ZbxcsUBYqTCsdEPPHsJJ96Vcns889gHTqSrTZoyrCd5E9NSe9XxLivK6izETniNp1Gu1DtrhVwv3VuZ3e", "gs1qqpvvvphd2zkphxxckzef2lgag67gzpz85alcemkxzvpl5tkgc8p34qpqwg58jx532dpkk0qtysnyudzg4ajk0vvqp8w9zlgjamxxz2l9cpt7qqqyd80pl"),
    std::make_pair("PPARTKMMf4AUDYzRSBcXSJZALbUXgWKHi6qdpy95yBmABuznU3keHFyNHfjaMT33ehuYwjx3RXort1j8d9AYnqyhAvdN168J4GBsM2ZHuTb91rsj", "gep1qqqqqqqqqqqqqqqcpjvcv9trdnv8t2nscuw056mm74cps7jkmrrdq48xpdjy6ylf6vpru36q6zax883gjclngas40d7097mudl05y48ewzvulpnsk5z75kgufmdvt"),
};

std::vector<std::string> testsFail = {
    "gh1z2kuclaye2kthndy7mdpw3zk0nck78a7u6h8hm",
    "grIv9h7c4k0e3axk7jlejzyh8tnc5eryx6e4ys8vz",
    "gs1qqpvvvphd2zkphxxckzef2lgag67gzpz85alcemkxzvpl5tkgc7p34qpqwg58jx532dpkk0qtysnyudzg4ajk0vvqp8w9zlgjamxxz2l9cpt7qqqflt7zu",
    "gep1qqqqqqqqqqqqqqcpjvcv9trdnv8t2nscuw056mm74cps7jkmrrdq48xpdjy6ylf6vpru36q6zax883gjclngas40d7097mudl05y48ewzvulpnsk5z75kg24d5nf",
};

std::vector<std::pair<CChainParams::Base58Type, std::string>> testsType = {
    std::make_pair(CChainParams::PUBKEY_ADDRESS, "gh1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqn06f76"),
    std::make_pair(CChainParams::SCRIPT_ADDRESS, "gr1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqmsspcj"),
    std::make_pair(CChainParams::PUBKEY_ADDRESS_256, "gl1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqkv5vrl"),
    std::make_pair(CChainParams::SCRIPT_ADDRESS_256, "gj1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq96alyv"),
    std::make_pair(CChainParams::SECRET_KEY, "gx1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq8rta3w"),
    std::make_pair(CChainParams::EXT_PUBLIC_KEY, "gep1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqz0r4sv"),
    std::make_pair(CChainParams::EXT_SECRET_KEY, "gex1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqgfllrx"),
    std::make_pair(CChainParams::STEALTH_ADDRESS, "gs1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqsw824e"),
    std::make_pair(CChainParams::EXT_KEY_HASH, "gek1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqpw7km0"),
    std::make_pair(CChainParams::EXT_ACC_HASH, "gea1qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqaa92jn"),
};


BOOST_AUTO_TEST_CASE(bech32_test)
{
    CBitcoinAddress addr_base58;
    CBitcoinAddress addr_bech32;

    for (auto &v : testsPass)
    {
        addr_base58.SetString(v.first);
        CTxDestination dest = addr_base58.Get();
        BOOST_CHECK(addr_bech32.Set(dest, true));
        BOOST_CHECK(addr_bech32.ToString() == v.second);

        CTxDestination dest2 = addr_bech32.Get();
        BOOST_CHECK(dest == dest2);
        CBitcoinAddress t58_back(dest2);
        BOOST_CHECK(t58_back.ToString() == v.first);

        CBitcoinAddress addr_bech32_2(v.second);
        BOOST_CHECK(addr_bech32_2.IsValid());
        CTxDestination dest3 = addr_bech32_2.Get();
        BOOST_CHECK(dest == dest3);
    };

    for (auto &v : testsFail)
    {
        BOOST_CHECK(!addr_bech32.SetString(v));
    };

    CKeyID knull;
    for (auto &v : testsType)
    {
        CBitcoinAddress addr;
        addr.Set(knull, v.first, true);
        BOOST_CHECK(addr.ToString() == v.second);
    };
}

BOOST_AUTO_TEST_CASE(bip173_testvectors_valid)
{
    static const std::string CASES[] = {
        "A12UEL5L",
        "a12uel5l",
        "an83characterlonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1tt5tgs",
        "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw",
        "11qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqc8247j",
        "split1checkupstagehandshakeupstreamerranterredcaperred2y9e3w",
        "?1ezyfcl",
    };
    for (const std::string& str : CASES) {
        auto ret = bech32::Decode(str);
        BOOST_CHECK(!ret.first.empty());
        std::string recode = bech32::Encode(ret.first, ret.second);
        BOOST_CHECK(!recode.empty());
        BOOST_CHECK(CaseInsensitiveEqual(str, recode));
    }
}

BOOST_AUTO_TEST_CASE(bip173_testvectors_invalid)
{
    static const std::string CASES[] = {
        " 1nwldj5",
        "\x7f""1axkwrx",
        "\x80""1eym55h",
        "an84characterslonghumanreadablepartthatcontainsthenumber1andtheexcludedcharactersbio1569pvx",
        "pzry9x0s0muk",
        "1pzry9x0s0muk",
        "x1b4n0q5v",
        "li1dgmt3",
        "de1lg7wt\xff",
        "A1G7SGD8",
        "10a06t8",
        "1qzzfhee",
        "a12UEL5L",
        "A12uEL5L",
    };
    for (const std::string& str : CASES) {
        auto ret = bech32::Decode(str);
        BOOST_CHECK(ret.first.empty());
    }
}

BOOST_AUTO_TEST_SUITE_END()
