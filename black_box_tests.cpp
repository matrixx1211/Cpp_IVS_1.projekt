//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     MAREK BITOMSKÝ <xbitom00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author MAREK BITOMSKÝ
 *
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

using namespace ::testing;
//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//

class EmptyTree : public Test
{
protected:
	BinaryTree tree;
};

class NonEmptyTree : public Test
{
	void SetUp()
	{
		for (size_t i = 5; i < 105; i += 10)
			tree.InsertNode(i);
	}

protected:
	BinaryTree tree;
};

class TreeAxioms : public Test
{
	void SetUp()
	{
		for (size_t i = 5; i < 205; i += 10)
			tree.InsertNode(i);
	}

protected:
	BinaryTree tree;
};

// EmptyTree testy pro rozhraní

TEST_F(EmptyTree, InsertNode)
{
	ASSERT_EQ(tree.GetRoot(), nullptr);
	auto firstItem = tree.InsertNode(10);
	EXPECT_TRUE(firstItem.first);
	ASSERT_NE(tree.GetRoot(), nullptr);

	auto secondItem = tree.InsertNode(10);
	EXPECT_FALSE(secondItem.first);
	EXPECT_EQ(firstItem.second, secondItem.second);
}

TEST_F(EmptyTree, DeleteNode)
{
	ASSERT_EQ(tree.GetRoot(), nullptr);
	auto deleted = tree.DeleteNode(10);
	EXPECT_FALSE(deleted);
	ASSERT_EQ(tree.GetRoot(), nullptr);
}

TEST_F(EmptyTree, FindNode)
{
	ASSERT_EQ(tree.GetRoot(), nullptr);
	ASSERT_EQ(tree.FindNode(10), nullptr);
	ASSERT_EQ(tree.GetRoot(), nullptr);
}

// NonEmptyTree testy

TEST_F(NonEmptyTree, InsertNode)
{
	ASSERT_NE(tree.GetRoot(), nullptr);

	auto item = tree.InsertNode(5);
	EXPECT_EQ(item.first, false);
	EXPECT_EQ(item.second, tree.InsertNode(5).second);

	item = tree.InsertNode(69);
	EXPECT_EQ(item.first, true);
}

TEST_F(NonEmptyTree, DeleteNode)
{
	auto deleted = tree.DeleteNode(5);
	EXPECT_TRUE(deleted);

	deleted = tree.DeleteNode(5);
	EXPECT_FALSE(deleted);
}

TEST_F(NonEmptyTree, FindNode)
{
	ASSERT_NE(tree.FindNode(5), nullptr);

	tree.DeleteNode(5);

	ASSERT_EQ(tree.FindNode(5), nullptr);
}

// TreeAxioms testy

TEST_F(TreeAxioms, Axiom1)
{
	std::vector<Node_t *> nodes;
	tree.GetLeafNodes(nodes);

	for (auto node : nodes)
		EXPECT_EQ(node->color, tree.BLACK);
}

TEST_F(TreeAxioms, Axiom2)
{
	// Pokud je uzel „červený”, pak jsou jeho oba potomci „černé”.
	std::vector<Node_t *> nodes;
	tree.GetNonLeafNodes(nodes);

	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (nodes[i]->color == tree.RED)
		{
			EXPECT_EQ(nodes[i]->pRight->color, tree.BLACK);
			EXPECT_EQ(nodes[i]->pLeft->color, tree.BLACK);
		}
	}
}

TEST_F(TreeAxioms, Axiom3)
{
	// Každá cesta od každého listového uzlu ke kořeni obsahuje stejný počet „černých” uzlů (vizte oranžové cesty v obrázku).
	std::vector<Node_t *> nodes;
	tree.GetLeafNodes(nodes);

	int refCount = 0, currCount = 0;

	Node_t *node = nodes[0];

	// průchod od listu až po kořen - zjištění referenční hodnoty	
	while (node != nullptr)
	{
		if (node->color == tree.BLACK)
			refCount++;
		node = node->pParent;
	}

	// průchod od listu až po kořen všech listů
	for (auto &leaf : nodes)
	{
		currCount = 0;
		node = leaf;
		while (node != nullptr)
		{
			if (node->color == tree.BLACK)
				currCount++;
			node = node->pParent;
		}
		EXPECT_EQ(refCount, currCount);
	}
}

/*** Konec souboru black_box_tests.cpp ***/
