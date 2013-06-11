/******************************************************************************
 * File: UnionFindTest.h
 *
 * Eric Beach
 *
 * Prototype of a UnionFind data structure (i.e., Disjoint-set data structure)
 * http://en.wikipedia.org/wiki/Disjoint-set_data_structure
 */

#ifndef Trailblazer_UnionFindTest_h
#define Trailblazer_UnionFindTest_h

////////// UNIT TESTS //////////
void runUnionFindUnitTests() {
    UnionFind uFind(6, 2);

    // row, col
    Loc a = makeLoc(0, 0);
    Loc b = makeLoc(0, 1);
    Loc c = makeLoc(1, 0);
    Loc d = makeLoc(1, 1);
    Loc e = makeLoc(2, 0);
    Loc f = makeLoc(2, 1);

    uFind.makeSet(a);
    uFind.makeSet(b);
    uFind.makeSet(c);
    uFind.makeSet(d);
    uFind.makeSet(e);
    uFind.makeSet(f);

    if (uFind.find(a) == uFind.find(b)) error("find function errored");
    if (uFind.find(b) == uFind.find(c)) error("find function errored");
    if (uFind.find(c) == uFind.find(d)) error("find function errored");
    if (uFind.find(d) == uFind.find(e)) error("find function errored");
    if (uFind.find(e) == uFind.find(f)) error("find function errored");
    if (uFind.find(f) == uFind.find(a)) error("find function errored");
    if (uFind.find(f) == uFind.find(b)) error("find function errored");
    if (uFind.find(f) == uFind.find(c)) error("find function errored");
    if (uFind.find(f) == uFind.find(d)) error("find function errored");
    if (uFind.find(f) == uFind.find(e)) error("find function errored");
    if (uFind.find(d) == uFind.find(c)) error("find function errored");
    if (uFind.find(d) == uFind.find(b)) error("find function errored");
    if (uFind.find(d) == uFind.find(a)) error("find function errored");
    if (uFind.find(c) == uFind.find(a)) error("find function errored");
    
    uFind.join(a, b);
    if (uFind.find(a) != uFind.find(b)) error("join function errored");
    
    uFind.join(a, c);
    if (uFind.find(a) != uFind.find(c)) error("join function errored");
    if (uFind.find(b) != uFind.find(c)) error("join function errored");
    if (uFind.find(c) == uFind.find(d)) error("find function errored");
    if (uFind.find(a) == uFind.find(e)) error("find function errored");
    if (uFind.find(a) == uFind.find(f)) error("find function errored");
    
    uFind.join(c, d);
    if (uFind.find(a) != uFind.find(d)) error("join function errored");
}

#endif
