#include "strategyselector.h"

StrategySelector::StrategySelector(QObject *parent) :
  QObject(parent)
{
}

void StrategySelector::startScan()
{
  //if alot of files: one thread-one file with all signatures
  //one file: check size - small: one thread, big - thread pool each thread-piece of signatures
  // ...
}

void StrategySelector::stopScan()
{

}
