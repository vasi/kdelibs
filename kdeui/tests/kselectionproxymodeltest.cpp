/*
    Copyright (c) 2010 Stephen Kelly <steveire@gmail.com>

    This library is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This library is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#include "kselectionproxymodeltestsuite.h"

#include <QTimer>
#include <QtTest>

#include "../itemviews/kselectionproxymodel.h"

#include "modelselector.h"

//BEGIN NoSelection

/**
  When there is no selection, everything from the source model should be a noop in the proxy.
*/
template<KSelectionProxyModel::FilterBehavior _filterBehaviour>
class TestData<NoSelectionStrategy, _filterBehaviour> : public NoSelectionStrategy
{
public:
  TestData(ProxyModelTest *proxyModelTest)
  : NoSelectionStrategy(proxyModelTest), m_proxyModelTest(proxyModelTest)
  { }

  KSelectionProxyModel::FilterBehavior filterBehaviour() { return _filterBehaviour; }

  void testInsertWhenEmptyData() { noop_testInsertWhenEmptyData(); }
  void testInsertInRootData() { noop_testInsertInRootData(); }
  void testInsertInTopLevelData() { noop_testInsertInTopLevelData(); }
  void testInsertInSecondLevelData() { noop_testInsertInSecondLevelData(); }

  void testRemoveFromRootData() { noop_testRemoveFromRootData(); }
  void testRemoveFromTopLevelData() { noop_testRemoveFromTopLevelData(); }
  void testRemoveFromSecondLevelData() { noop_testRemoveFromSecondLevelData(); }

private:
  ProxyModelTest *m_proxyModelTest;
};

//END NoSelection

//BEGIN ImmediateSelectionStrategy

//BEGIN ChildrenOfExactSelection
template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testInsertWhenEmptyData()
{
  noop_testInsertWhenEmptyData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testInsertInRootData()
{
  noop_testInsertInRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testInsertInTopLevelData()
{
  QTest::addColumn<SignalList>("signalList");
  QTest::addColumn<PersistentChangeList>("changeList");

  IndexFinder indexFinder;

  newInsertTest("insert01", indexFinder, 0, 0, 10);
  newInsertTest("insert02", indexFinder, 0, 9, 10);
  newInsertTest("insert03", indexFinder, 10, 10, 10);
  newInsertTest("insert04", indexFinder, 10, 19, 10);
  newInsertTest("insert05", indexFinder, 4, 4, 10);
  newInsertTest("insert06", indexFinder, 4, 13, 10);
  newInsertTest("insert07", indexFinder, 0, 0, 10);
  newInsertTest("insert08", indexFinder, 10, 10, 10);
  newInsertTest("insert09", indexFinder, 4, 4, 10);
  newInsertTest("insert10", indexFinder, 0, 4, 10);
  newInsertTest("insert11", indexFinder, 10, 14, 10);
  newInsertTest("insert12", indexFinder, 4, 8, 10);
  noopTest("insert13");
  noopTest("insert14");
  noopTest("insert15");
  noopTest("insert16");
  noopTest("insert17");
  noopTest("insert18");
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testInsertInSecondLevelData()
{
  noop_testInsertInSecondLevelData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testRemoveFromRootData()
{
  noop_testRemoveFromRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testRemoveFromTopLevelData()
{
  QTest::addColumn<SignalList>("signalList");
  QTest::addColumn<PersistentChangeList>("changeList");

  IndexFinder indexFinder;

  newRemoveTest("remove01", indexFinder, 0, 0, 10);
  newRemoveTest("remove02", indexFinder, 0, 4, 10);
  newRemoveTest("remove03", indexFinder, 9, 9, 10);
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ChildrenOfExactSelection>::testRemoveFromSecondLevelData()
{
  noop_testRemoveFromSecondLevelData();
}

//END ChildrenOfExactSelection

//BEGIN ExactSelection
template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testInsertWhenEmptyData()
{
  QTest::addColumn<SignalList>("signalList");
  QTest::addColumn<PersistentChangeList>("changeList");

  IndexFinder indexFinder;

  noopTest("insert01");
  newInsertTest("insert02", indexFinder, 0, 0, 0);
  noopTest("insert03");
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testInsertInRootData()
{
  noop_testInsertInRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testInsertInTopLevelData()
{
  noop_testInsertInTopLevelData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testInsertInSecondLevelData()
{
  noop_testInsertInSecondLevelData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testRemoveFromRootData()
{
  noop_testRemoveFromRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testRemoveFromTopLevelData()
{
  noop_testRemoveFromTopLevelData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::ExactSelection>::testRemoveFromSecondLevelData()
{
  noop_testRemoveFromSecondLevelData();
}
//END ExactSelection

//BEGIN SubTrees
template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testInsertWhenEmptyData()
{
  QTest::addColumn<SignalList>("signalList");
  QTest::addColumn<PersistentChangeList>("changeList");

  IndexFinder indexFinder;

  noopTest("insert01");
  newInsertTest("insert02", indexFinder, 0, 0, 0);
  noopTest("insert03");
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testInsertInRootData()
{
  noop_testInsertInRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testInsertInTopLevelData()
{
  testForwardingInsertData(IndexFinder(QList<int>() << 0));
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testInsertInSecondLevelData()
{
  testForwardingInsertData(IndexFinder(QList<int>() << 0 << 5));
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testRemoveFromRootData()
{
  noop_testRemoveFromRootData();
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testRemoveFromTopLevelData()
{
  testForwardingRemoveData(IndexFinder(QList<int>() << 0));
}

template<>
void TestData<ImmediateSelectionStrategy<9>, KSelectionProxyModel::SubTrees>::testRemoveFromSecondLevelData()
{
  testForwardingRemoveData(IndexFinder(QList<int>() << 0 << 5));
}

//END SubTrees

//END ImmediateSelectionStrategy



PROXYMODELTEST_MAIN(SelectionProxyModelTest,
  SELECTIONPROXYMODELSIMPLETEST(NoSelectionStrategy, KSelectionProxyModel::ChildrenOfExactSelection)
  SELECTIONPROXYMODELSIMPLETEST(NoSelectionStrategy, KSelectionProxyModel::ExactSelection)
  SELECTIONPROXYMODELSIMPLETEST(NoSelectionStrategy, KSelectionProxyModel::SubTreeRoots)
  SELECTIONPROXYMODELSIMPLETEST(NoSelectionStrategy, KSelectionProxyModel::SubTrees)
  SELECTIONPROXYMODELSIMPLETEST(NoSelectionStrategy, KSelectionProxyModel::SubTreesWithoutRoots)

  SELECTIONCOMPLETETEST1(ImmediateSelectionStrategy, 9, KSelectionProxyModel::ChildrenOfExactSelection)
  SELECTIONCOMPLETETEST1(ImmediateSelectionStrategy, 9, KSelectionProxyModel::ExactSelection)
  SELECTIONCOMPLETETEST1(ImmediateSelectionStrategy, 9, KSelectionProxyModel::SubTrees)
)