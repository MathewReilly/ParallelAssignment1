#include <iostream>
#include <thread>
#include <mutex>
#include <cmath>
#include <typeinfo>
#include <chrono>
#include <vector>
#include <set>
using namespace std;

mutex counterMutex;

// How large of n are we using?
vector<bool> nVector(100000000);
int64_t sizeOfN;
int64_t sqrtOfN;

//ticket counter implementation from Lecture 1
class Counter
{
private: int64_t value;

public:
  Counter() : value(1) { }
  Counter(int64_t initValue) : value(initValue) { }

  int64_t getAndIncrement()
  {
    // this is the CRITICAL SECTION of the code, where other threads cannot access
    // "value", as it is a shared resource that is being changed.
    counterMutex.lock();
    int64_t temp = value;
    value = temp + 1;
    counterMutex.unlock();

    return temp;
  }

  int64_t get()
  {
    return value;
  }

  // must not be performed with threading, okay to use before
  void increment()
  {
    value++;
  }
};

//should these variables be in global space?!?!?
Counter compositeCounter(1);

bool isPrime(int64_t potentialPrime)
{
  // We need to factorize our potentialPrime value
  // we will set our initial condition by finding the square root of our value
  // this allows us to limit the total potential factors to check by skipping duplicates
  // although cieling is more accurate it accrues more work???? - needs to be tested with cieling
  int64_t totalFactorsToCheck = static_cast<int64_t>(sqrt(potentialPrime)) + 1;

  //int64_t totalFactorsToCheck = potentialPrime / 2;

  if(potentialPrime == 2)
  {
    return true;
  } else if (potentialPrime == 1)
  {
    return false;
  }

  for(int i = 2; i <= totalFactorsToCheck; i++)
  {
    if(potentialPrime % i == 0)
    {
      return false; // we found a factor
    }
  }
  return true;
}

mutex vectorMutex;


void markComposites()
{

  //set<int64_t> threadComposites;

  // cout << " threads begin " << endl;
  // this could grab something one below and then get to later code and go above limit...
  int64_t j = compositeCounter.getAndIncrement(); // i think this can still be wrong with mutual exclusion...
  // This will be where we mark composites
  // cout << " gets j " << endl;
  while(j <= sqrtOfN)
  {

    vector<int64_t> curComposites;

    if(isPrime(j))
    {
      // mark all composites
      for(int64_t i = j + j; i < sizeOfN; i = i + j)
      {
        curComposites.push_back(i);
      }
    } else
    {
      // mark non-prime
      curComposites.push_back(j);
    }

    vectorMutex.lock();
    for(int64_t indexComposites = 0; indexComposites < curComposites.size(); indexComposites++)
    {
      nVector[curComposites[indexComposites]] = 1;
    }
    vectorMutex.unlock();

    j = compositeCounter.getAndIncrement();
  }

}

int64_t totalPrimes = 0;
int64_t sumPrimes = 0;

// this is a helper function setting values so threads to not need to repeat this work
void prepForFindingPrimes()
{
  // get the size of your n values to check for primes
  sizeOfN = nVector.size();
  cout << sizeOfN << endl;
  // get sqrt(n) + 1 of primes to use for Eratosthenes
  sqrtOfN = sqrt(sizeOfN) + 1;
}


int main()
{

  // This prepares the necessary data for performing Eratosthenes because
  // they are global.
  prepForFindingPrimes();

  vector<int64_t> largestPrimes;
  int primeCounter = 10;

  auto start = chrono::high_resolution_clock::now();

  thread t1(markComposites);
  thread t2(markComposites);
  thread t3(markComposites);
  thread t4(markComposites);
  thread t5(markComposites);
  thread t6(markComposites);
  thread t7(markComposites);
  thread t8(markComposites);

  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();
  t7.join();
  t8.join();

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  for(int64_t getPrimeData = sizeOfN - 1; getPrimeData > 0; getPrimeData--)
  {
    if(nVector[getPrimeData] == 0)
    {
      if(primeCounter > 0)
      {
        largestPrimes.push_back(getPrimeData);
        primeCounter--;
      }

      //cout << getPrimeData << endl;
      totalPrimes++;
      sumPrimes = sumPrimes + getPrimeData;
    }
  }

  cout << duration.count() << " " << totalPrimes << " " << sumPrimes << endl;

  for(int lp = largestPrimes.size() - 1; lp >= 0; lp--)
  {
    cout << largestPrimes[lp] << " ";
  }

  return 0;
}
