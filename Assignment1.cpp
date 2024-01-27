// Mathew Reilly
// COP4520
// Assignment 1
// Description: This is a program that finds all of the primes from 1 to 10^8 using
//              multithreading.
//
//              The output is printed to a "primes.txt" file where the first
//              value is the time in seconds the threads took to complete,
//              the second value is the number of primes found, and the third
//              value is the sum of all of the primes. The following line is
//              the top ten primes listed lowest to highest.
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <cmath>
#include <typeinfo>
#include <chrono>
#include <vector>
#include <set>
using namespace std;

mutex counterMutex;

// Where we input the size we would like to check for primes
// this vector will mark all of the composite values, leaving only the primes
// unmarked.
vector<bool> nVector(100000000);
int64_t sizeOfN;
int64_t sqrtOfN;

//ticket counter implementation related to Lecture 1 slides
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
};

// Global counter distibuting tasks to threads.
Counter compositeCounter(1);

bool isPrime(int64_t potentialPrime)
{
  // We need to factorize our potentialPrime value
  // we will set our initial condition by finding the square root of our value
  // this allows us to limit the total potential factors to check by skipping duplicates
  int64_t totalFactorsToCheck = static_cast<int64_t>(sqrt(potentialPrime)) + 1;

  // Trivial cases done on their own
  if(potentialPrime == 2)
  {
    return true;
  } else if (potentialPrime == 1)
  {
    return false;
  }

  // look for factors, if one found return false, otherwise it is prime
  for(int i = 2; i <= totalFactorsToCheck; i++)
  {
    if(potentialPrime % i == 0)
    {
      return false;
    }
  }
  return true;
}

//This lock is used to protect the shared global vector tracking composites.
mutex vectorMutex;

void markComposites()
{

  // will grab the threads initial value to check and or mark composites from
  int64_t j = compositeCounter.getAndIncrement();

  // This will be where we mark composites
  while(j <= sqrtOfN)
  {

    // a temporary vector to make all of the composites of an initial prime.
    // used to divide the work of updating the composite vector with lock
    vector<int64_t> curComposites;

    // if we find a prime value in the range of 1 to (sqrt(N) + 1), mark all of
    // the composites of it, otherwise, mark that value as a non-prime value.
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

    // once we've gathered all of the composites related to a prime, or marked an
    // single composite value, update our main list of composites with this information
    vectorMutex.lock();
    for(int64_t indexComposites = 0; indexComposites < curComposites.size(); indexComposites++)
    {
      nVector[curComposites[indexComposites]] = 1;
    }
    vectorMutex.unlock();

    // grab our next value to check before continuing.
    j = compositeCounter.getAndIncrement();
  }

}

int64_t totalPrimes = 0;
int64_t sumPrimes = 0;

// this is a helper function setting values relating to the vector's dimensions
void prepForFindingPrimes()
{
  // get the size of your n values to check for primes
  sizeOfN = nVector.size();
  //cout << sizeOfN << endl;
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

  // start the clock and run the threads to completion
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

  // calculate time in seconds
  double timeSeconds = duration.count() / 1000000.0;

  // make a list of our ten largest primes, count the number of primes, and the sum
  // of primes by iterating through the vecor of marked composites
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

  // Create the output file primes.txt
  ofstream primesPrintFile("primes.txt");

  // print the time in seconds, the number of primes found, and the sum of the primes
  primesPrintFile << timeSeconds << " " << totalPrimes << " " << sumPrimes << endl;

  // print the 10 largest from smallest to largest
  for(int lp = largestPrimes.size() - 1; lp >= 0; lp--)
  {
    primesPrintFile << largestPrimes[lp] << " ";
  }

  primesPrintFile.close();

  return 0;
}
