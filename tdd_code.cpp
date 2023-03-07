//======== Copyright (c) 2023, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Test Driven Development - graph
//
// $NoKeywords: $ivs_project_1 $tdd_code.cpp
// $Author:     MAREK BITOMSKÝ <xbitom00@stud.fit.vutbr.cz>
// $Date:       $2023-03-07
//============================================================================//
/**
 * @file tdd_code.cpp
 * @author Martin Dočekal
 * @author Karel Ondřej
 *
 * @brief Implementace metod tridy reprezentujici graf.
 */

#include "tdd_code.h"

Graph::Graph()
{
    allNodes = {};
    allEdges = {};
}

Graph::~Graph()
{
    while (!allNodes.empty())
    {
        Node *node = allNodes.back();
        allNodes.pop_back();
        delete node;
    }

    allEdges.clear();
}

std::vector<Node *> Graph::nodes()
{
    return allNodes;
}

std::vector<Edge> Graph::edges() const
{
    return allEdges;
}

Node *Graph::addNode(size_t nodeId)
{
    // kontrola existence stejného uzlu
    if (this->getNode(nodeId))
        return nullptr;

    // vytvoření nového uzlu
    Node *newNode = new Node();
    newNode->color = 0;
    newNode->id = nodeId;
    newNode->degree = 0;

    // vložení do vektoru
    allNodes.push_back(newNode);

    return newNode;
}

bool Graph::addEdge(const Edge &edge)
{
    // smyčka
    if (edge.a == edge.b)
        return false;

    // duplicita
    if (containsEdge(edge))
        return false;

    // vytvoření uzlu pokud není již vytvořen
    addNode(edge.a);
    addNode(edge.b);

    Node *a = this->getNode(edge.a);
    Node *b = this->getNode(edge.b);

    // přídání stupně
    a->degree++;
    b->degree++;

    // přidání sousedů pro obarvení
    /* bool aHasB = false;
    bool bHasA = false;

    for (auto &aNeighs : a->neighbours)
        if (aNeighs->id = b->id)
            aHasB = true;
    for (auto &bNeighs : b->neighbours)
        if (bNeighs->id = a->id)
            bHasA = true;

    if (!aHasB)
        a->neighbours.push_back(b);
    if (!bHasA)
        b->neighbours.push_back(a); */

    // vložení do vektoru
    allEdges.push_back(edge);

    return true;
}

void Graph::addMultipleEdges(const std::vector<Edge> &edges)
{
    for (auto &edge : edges)
        addEdge(edge);
}

Node *Graph::getNode(size_t nodeId)
{
    for (auto &node : allNodes)
        if (node->id == nodeId)
            return node;

    return nullptr;
}

bool Graph::containsEdge(const Edge &edge) const
{
    for (auto &gEdge : allEdges)
        if ((gEdge.a == edge.a && gEdge.b == edge.b) || (gEdge.a == edge.b && gEdge.b == edge.a))
            return true;
    return false;
}

void Graph::removeNode(size_t nodeId)
{
    // hledám uzel s id 'nodeId'
    for (auto node = allNodes.begin(); node != allNodes.end(); node++)
        if ((*node)->id == nodeId)
        {
            // odstranení všech hran kde je uzel s id 'nodeId'
            for (auto gEdge = allEdges.begin(); gEdge != allEdges.end();)
                if (gEdge->a == nodeId || gEdge->b == nodeId)
                    allEdges.erase(gEdge);
                else
                    gEdge++;

            // po nalezení odeberu z vektoru a uvolním paměť
            delete (*node);
            allNodes.erase(node);
            return;
        }

    throw std::out_of_range("Node out of range.");
}

void Graph::removeEdge(const Edge &edge)
{
    // pro kontrolu zda existuje daná hrana
    bool hasEdge = false;

    // odstranění hrany
    for (auto gEdge = allEdges.begin(); gEdge != allEdges.end();)
        if ((gEdge->a == edge.a && gEdge->b == edge.b) || (gEdge->a == edge.b && gEdge->b == edge.a))
        {

            allEdges.erase(gEdge);
            hasEdge = true;
        }
        else
            gEdge++;

    // hrana existovala
    if (hasEdge)
        return;

    // hrana neexistovala
    throw std::out_of_range("Edge out of range.");
}

size_t Graph::nodeCount() const
{
    return allNodes.size();
}

size_t Graph::edgeCount() const
{
    return allEdges.size();
}

size_t Graph::nodeDegree(size_t nodeId) const
{
    for (auto &node : allNodes)
        if (node->id == nodeId)
            return node->degree;

    throw std::out_of_range("Node out of range");
}

size_t Graph::graphDegree() const
{
    size_t max = 0;
    for (auto &node : allNodes)
        if (node->degree > max)
            max = node->degree;

    return max;
}

void Graph::coloring()
{
    int maxColors = graphDegree() + 1;
    int currColor = 1;
    for (auto &node : allNodes)
    {
        node->color = currColor;
        if (currColor >= maxColors) {
            currColor = 0;
        }
        currColor++;
    }
}

void Graph::clear()
{
    while (!allNodes.empty())
    {
        Node *node = allNodes.back();
        allNodes.pop_back();
        delete node;
    }

    allEdges.clear();
}

/*** Konec souboru tdd_code.cpp ***/
