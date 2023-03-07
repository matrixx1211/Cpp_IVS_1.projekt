//======== Copyright (c) 2022, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - test suite
//
// $NoKeywords: $ivs_project_1 $white_box_tests.cpp
// $Author:     MAREK BITOMSKÝ <xbitom00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author MAREK BITOMSKÝ
 *
 * @brief Implementace testu hasovaci tabulky.
 */

#include <vector>

#include "gtest/gtest.h"

#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy hasovaci tabulky, testujte nasledujici:
// 1. Verejne rozhrani hasovaci tabulky
//     - Vsechny funkce z white_box_code.h
//     - Chovani techto metod testuje pro prazdnou i neprazdnou tabulku.
// 2. Chovani tabulky v hranicnich pripadech
//     - Otestujte chovani pri kolizich ruznych klicu se stejnym hashem
//     - Otestujte chovani pri kolizich hashu namapovane na stejne misto v
//       indexu
//============================================================================//

/*** Konec souboru white_box_tests.cpp ***/
using namespace ::testing;

class EmptyHashMap : public Test
{
	void SetUp()
	{
		hashMap = hash_map_ctor();
	}

	void TearDown()
	{
		if (hashMap != nullptr)
			hash_map_dtor(hashMap);
	}

protected:
	hash_map_t *hashMap;
};

class NonEmptyHashMap : public Test
{
	void SetUp()
	{
		hashMap = hash_map_ctor();
		hash_map_put(hashMap, "Test", 1);
		hash_map_put(hashMap, "Testi", 1);
		hash_map_put(hashMap, "Testin", 1);
		hash_map_put(hashMap, "Testing", 1);
	}
	void TearDown()
	{
		if (hashMap != nullptr)
			hash_map_dtor(hashMap);
	}

protected:
	hash_map_t *hashMap;
};

class HashMapWithCollisions : public Test
{
	void SetUp()
	{
		hashMap = hash_map_ctor();
		hash_map_put(hashMap, "Test", 1);
		hash_map_put(hashMap, "tesT", 1);
		hash_map_put(hashMap, "setT", 1);
		hash_map_put(hashMap, "Tset", 1);
	}
	void TearDown()
	{
		if (hashMap != nullptr)
			hash_map_dtor(hashMap);
	}

protected:
	hash_map_t *hashMap;
};

// prázdná hash mapa
TEST_F(EmptyHashMap, hash_map_ctor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
}
TEST_F(EmptyHashMap, hash_map_dtor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
	hash_map_dtor(hashMap);
	hashMap = nullptr; // pro TearDown, aby nedošlo k přístupu na neexistující místo
}
TEST_F(EmptyHashMap, hash_map_clear)
{
	hash_map_clear(hashMap);
	EXPECT_EQ(hashMap->first, nullptr);
	EXPECT_EQ(hashMap->last, nullptr);
	EXPECT_EQ(hashMap->used, 0);
}
TEST_F(EmptyHashMap, hash_map_reserve)
{
	EXPECT_EQ(hashMap->allocated, 8);
	EXPECT_EQ(hash_map_reserve(hashMap, 16), hash_map_state_code_t::OK);
	EXPECT_EQ(hashMap->allocated, 16);
	EXPECT_EQ(hash_map_reserve(hashMap, 1), hash_map_state_code_t::OK);
	EXPECT_EQ(hashMap->allocated, 1);
	EXPECT_EQ(hash_map_reserve(hashMap, 1), hash_map_state_code_t::OK);
	EXPECT_EQ(hashMap->allocated, 1);
	EXPECT_EQ(hash_map_reserve(hashMap, -1), hash_map_state_code_t::MEMORY_ERROR);
}
TEST_F(EmptyHashMap, hash_map_size)
{
	EXPECT_EQ(hash_map_size(hashMap), 0);
}
TEST_F(EmptyHashMap, hash_map_capacity)
{
	EXPECT_EQ(hash_map_capacity(hashMap), 8);
}
TEST_F(EmptyHashMap, hash_map_contains)
{
	EXPECT_FALSE(hash_map_contains(hashMap, "neexistuje"));
}
TEST_F(EmptyHashMap, hash_map_put)
{
	EXPECT_EQ(hash_map_put(hashMap, "novy", 69), hash_map_state_code_t::OK);
	EXPECT_EQ(hash_map_put(hashMap, "novy", 70), hash_map_state_code_t::KEY_ALREADY_EXISTS);
}
TEST_F(EmptyHashMap, hash_map_get)
{
	int value;
	EXPECT_EQ(hash_map_get(hashMap, "neexistuje", &value), hash_map_state_code_t::KEY_ERROR);
}
TEST_F(EmptyHashMap, hash_map_pop)
{
	int value;
	EXPECT_EQ(hash_map_pop(hashMap, "neexistuje", &value), hash_map_state_code_t::KEY_ERROR);
}
TEST_F(EmptyHashMap, hash_map_remove)
{
	EXPECT_EQ(hash_map_remove(hashMap, "neexistuje"), hash_map_state_code_t::KEY_ERROR);
}

// neprázdná hash mapa
TEST_F(NonEmptyHashMap, hash_map_ctor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
}
TEST_F(NonEmptyHashMap, hash_map_dtor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
	hash_map_dtor(hashMap);
	hashMap = nullptr; // pro TearDown, aby nedošlo k přístupu na neexistující místo
}
TEST_F(NonEmptyHashMap, hash_map_clear)
{
	hash_map_clear(hashMap);
	EXPECT_EQ(hashMap->first, nullptr);
	EXPECT_EQ(hashMap->last, nullptr);
	EXPECT_EQ(hashMap->used, 0);
}
TEST_F(NonEmptyHashMap, hash_map_reserve)
{
	EXPECT_EQ(hashMap->allocated, 8);
	EXPECT_EQ(hash_map_reserve(hashMap, 16), hash_map_state_code_t::OK);
	EXPECT_EQ(hashMap->allocated, 16);
	EXPECT_EQ(hash_map_reserve(hashMap, 1), hash_map_state_code_t::VALUE_ERROR);
}
TEST_F(NonEmptyHashMap, hash_map_size)
{
	EXPECT_EQ(hash_map_size(hashMap), 4);
}
TEST_F(NonEmptyHashMap, hash_map_capacity)
{
	EXPECT_EQ(hash_map_capacity(hashMap), 8);
	EXPECT_EQ(hash_map_put(hashMap, "reserve1", 42), hash_map_state_code_t::OK);
	EXPECT_EQ(hash_map_put(hashMap, "reserve2", 69), hash_map_state_code_t::OK);
	EXPECT_EQ(hash_map_capacity(hashMap), 8<<1);
}
TEST_F(NonEmptyHashMap, hash_map_contains)
{
	EXPECT_FALSE(hash_map_contains(hashMap, "Tes"));
	EXPECT_TRUE(hash_map_contains(hashMap, "Test"));
}
TEST_F(NonEmptyHashMap, hash_map_put)
{
	EXPECT_EQ(hash_map_put(hashMap, "Test", 70), hash_map_state_code_t::KEY_ALREADY_EXISTS);
	EXPECT_EQ(hash_map_put(hashMap, "novy", 69), hash_map_state_code_t::OK);
}
TEST_F(NonEmptyHashMap, hash_map_get)
{
	int value;
	EXPECT_EQ(hash_map_get(hashMap, "Testing", &value), hash_map_state_code_t::OK);
	EXPECT_EQ(value, 1);
}
TEST_F(NonEmptyHashMap, hash_map_pop)
{
	int value;
	EXPECT_EQ(hash_map_pop(hashMap, "Testing", &value), hash_map_state_code_t::OK);
	EXPECT_EQ(value, 1);
}
TEST_F(NonEmptyHashMap, hash_map_remove)
{
	EXPECT_EQ(hash_map_remove(hashMap, "neexistuje"), hash_map_state_code_t::KEY_ERROR);
}

// neprázdná hash mapa s kolizí
TEST_F(HashMapWithCollisions, hash_map_ctor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
}
TEST_F(HashMapWithCollisions, hash_map_dtor)
{
	ASSERT_NE(hashMap, nullptr);
	EXPECT_EQ(hashMap->allocated, 8);
	hash_map_dtor(hashMap);
	hashMap = nullptr; // pro TearDown, aby nedošlo k přístupu na neexistující místo
}
TEST_F(HashMapWithCollisions, hash_map_clear)
{
	hash_map_clear(hashMap);
	EXPECT_EQ(hashMap->first, nullptr);
	EXPECT_EQ(hashMap->last, nullptr);
	EXPECT_EQ(hashMap->used, 0);
}
TEST_F(HashMapWithCollisions, hash_map_reserve)
{
	EXPECT_EQ(hashMap->allocated, 8);
	EXPECT_EQ(hash_map_reserve(hashMap, 16), hash_map_state_code_t::OK);
	EXPECT_EQ(hashMap->allocated, 16);
	EXPECT_EQ(hash_map_reserve(hashMap, 1), hash_map_state_code_t::VALUE_ERROR);
}
TEST_F(HashMapWithCollisions, hash_map_size)
{
	EXPECT_EQ(hash_map_size(hashMap), 4);
}
TEST_F(HashMapWithCollisions, hash_map_capacity)
{
	EXPECT_EQ(hash_map_capacity(hashMap), 8);
	EXPECT_EQ(hash_map_put(hashMap, "Reserve", 42), hash_map_state_code_t::OK);
	EXPECT_EQ(hash_map_put(hashMap, "reseRve", 69), hash_map_state_code_t::OK);
	EXPECT_EQ(hash_map_capacity(hashMap), 8<<1);
}
TEST_F(HashMapWithCollisions, hash_map_contains)
{
	EXPECT_FALSE(hash_map_contains(hashMap, "ReserveReserve"));
	EXPECT_FALSE(hash_map_contains(hashMap, "Reserve"));
	EXPECT_TRUE(hash_map_contains(hashMap, "setT"));
}
TEST_F(HashMapWithCollisions, hash_map_put)
{
	EXPECT_EQ(hash_map_put(hashMap, "setT", 70), hash_map_state_code_t::KEY_ALREADY_EXISTS);
	EXPECT_EQ(hash_map_put(hashMap, "reseRve", 69), hash_map_state_code_t::OK);
}
TEST_F(HashMapWithCollisions, hash_map_get)
{
	int value;
	EXPECT_EQ(hash_map_get(hashMap, "Tset", &value), hash_map_state_code_t::OK);
	EXPECT_EQ(value, 1);
}
TEST_F(HashMapWithCollisions, hash_map_pop)
{
	int value;
	EXPECT_EQ(hash_map_pop(hashMap, "Test", &value), hash_map_state_code_t::OK);
	EXPECT_EQ(value, 1);
}
TEST_F(HashMapWithCollisions, hash_map_remove)
{
	EXPECT_EQ(hash_map_remove(hashMap, "neexistuje"), hash_map_state_code_t::KEY_ERROR);
}