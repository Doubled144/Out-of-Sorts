/************************************************************//*
  Filename   : outOfSorts.cc
  Author     : Dominic J. DuRant
  Course     : CSCI 362-01
  Assignment : 8
  Description: Implementation of merge sort, quicksort, and Shell sort, and
              compare their performance. (Time in milliseconds and number of comparisons).
*/

/************************************************************/

/*
Sorting times and comparisons for Merge, Quick, Shell, and standard sort

N:       20,000,000                   40,000,000                   80,000,000    
======== ============================ ============================ =============================
Merge    9,814.26ms  (486,445,568)    20,441.58ms (1,012,891,136)  42,487.46ms  (2,105,782,272)
Quick    4,938.64ms  (444,561,319)    10,233.52ms (926,885,464)    21,244.55ms  (1,922,222,737)
Shell    19,555.52ms (2,191,136,952)  46,256.14ms (55,357,598,44)  106,691.99ms (13,122,382,223)
std      7,349.16ms                   15,216.19ms                  31,649.98ms
*/

/************************************************************/
// System includes

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <queue>
#include <random>
#include <bits/stdc++.h>

/************************************************************/
// Local includes

#include "Timer.hpp"

/************************************************************/
// Using declarations

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::uniform_int_distribution;
using std::setprecision;
using std::fixed;
using std::sort;
using std::swap;

/************************************************************/
// Function prototypes/global vars/typedefs

// Perform a merge sort on 'v'
// Return the number of comparisons performed
// Additionally, you'll want a helper function to allow for recursion
size_t
mergeSort (vector<int>& v);

// Perform a quicksort on 'v'
// Return the number of comparisons performed
// Additionally, you'll want a helper function to allow for recursion
size_t
quickSort (vector<int>& v);

// Perform an insertion sort from 'begin' up to but NOT including 'end'
// Return the number of comparisons performed
size_t
insertionSort (vector<int>& v, size_t begin, size_t end);

// Perform a Shell sort 
// Return the number of comparisons performed
size_t
shellSort (vector<int>& v);

// Performs the main algorithm for merge sort
size_t
mergeSortHelp (vector<int>& copy,vector<int>& temp, size_t first, size_t last);

// merges two vectors into one.
size_t
merge (vector<int>& v, vector<int>& temp,  int first, int mid, int last);

// Performs quicksort algorithm or insertion algorithm depending an vector size.
size_t
quickSortHelp (vector<int>& v, int first, int last);

// Finds the center location of a vector. 
size_t
median3 ( vector<int> & aCopy, int left, int right, size_t& numCompares);

// Partition algorithm for partioning the vector in quicksort. 
size_t
partition(vector<int>& aCopy, size_t first, size_t last, size_t& numCompares );

// Add random numbers to the initial vector between 0 and 1,000,000,000 inclusive.
void
fillVector (vector<int>& v);

/************************************************************/

int
main (int argc, char* argv[])
{
  size_t N; //number of integers to sort.

  cout << "N ==> ";
  cin  >> N;
  cout << endl;

  vector<int> v (N);
  Timer <> stopWatch;
  size_t compares = 0;
  fillVector (v);
  
  vector<int> merge (v);
  stopWatch.start ();
  compares = mergeSort(merge);
  stopWatch.stop ();
  cout << "Merge time: " << fixed << setprecision(2) << stopWatch.getElapsedMs () << "ms" << endl;
  cout << "Merge compares: " << compares << endl;
  
  cout << endl;

  vector<int> quick (v);
  stopWatch.start ();
  compares = quickSort (quick);
  stopWatch.stop ();
  cout << "Quick time: " << fixed << setprecision(2) << stopWatch.getElapsedMs () << "ms" << endl;
  cout << "Quick compares: " << compares << endl;

  cout << endl;

  vector<int> shell (v);
  stopWatch.start ();
  compares = shellSort (shell);
  stopWatch.stop ();
  cout << "Shell time: " << fixed << setprecision(2) << stopWatch.getElapsedMs () << "ms" << endl;
  cout << "Shell compares: " << compares << endl;

  cout << endl;

  stopWatch.start ();
  sort (v.begin (), v.end ());
  stopWatch.stop ();
  cout << "Std time: " << fixed << setprecision(2) << stopWatch.getElapsedMs () << "ms" << endl;

  cout << endl;

  if (merge == v)
  {
    cout << "Merge ok? true"<< endl;
  }
  else
  {
    cout << "Merge ok? false"<< endl;
  }
  
  if (quick == v)
  {
    cout << "Quick ok? true"<< endl;
  }
  else
  {
    cout << "Quick ok? false"<< endl;
  }

  if (shell == v)
  {
    cout << "Shell ok? true"<< endl;
  }
  else
  {
    cout << "Shell ok? false"<< endl;
  }
  
}

/************************************************************/

void
fillVector (vector<int>& v)
{
  std::mt19937 engine {std::random_device{}()};
  uniform_int_distribution<int> distribution(0, pow (10, 9));
  
  for (size_t i = 0; i < v.size (); ++i)
  {
    v[i] = distribution(engine);
  }
}

/************************************************************/

size_t
mergeSort (vector<int>& v)
{ 
  int numCompares = 0;
  vector<int> temp( v.size( ) );

  numCompares += mergeSortHelp (v, temp, 0, v.size ()-1);

  return numCompares;
}

/************************************************************/

size_t
mergeSortHelp (vector<int>& copy, vector<int>& temp, size_t first, size_t last)
{
  int numCompares = 0;
  if (first < last)
  {
    int mid = (first + last) / 2;
    numCompares += mergeSortHelp(copy, temp, first, mid);
    numCompares += mergeSortHelp(copy,temp, mid+1, last);
    numCompares += merge(copy, temp, first, mid +1, last);
  }
  return numCompares;
}

/************************************************************/

size_t
merge (vector<int>& copy, vector<int>& temp, int first, int mid, int last)
{
  int indexB = mid - 1;
  int indexA = first;
  int numElements = last - first + 1;
  int numCompares = 0;

  while(first <= indexB && mid <= last)
  {
    if( copy[ first ] <= copy[ mid ] )
    {
      temp[ indexA++ ] = std::move( copy[ first++ ] );
    }
    else
    {
      temp[ indexA++ ] = std::move( copy[ mid++ ] );
    }
    ++numCompares;
  }
  while(first <= indexB)
  {
    temp[indexA++] = std::move( copy[ first++ ] );
    ++numCompares;
  }
  while(mid <= last)
  {
    temp[ indexA++ ] = std::move( copy[ mid++ ] );
    ++numCompares;
  }
  for(int i = 0; i <= numElements; ++i, --last)
  {
    copy[last] = std::move( temp[ last ] );
  }
  return numCompares;
}

/************************************************************/

size_t
quickSort (vector<int>& v)
{
  return quickSortHelp (v, 0, v.size ( ));
}

/************************************************************/

size_t
quickSortHelp (vector<int>& a, int left, int right)
{
  size_t numCompares = 0;
  
  // if number of elements is more than 20
  if(left + 20 < right)
  {
    auto i = partition (a, left, right, numCompares); 
    numCompares += quickSortHelp (a, left, i);
    numCompares += quickSortHelp (a, i + 1, right);
  }  
  else // perform insertion when number of elements is 20 or less
  {
    numCompares += insertionSort(a, left, right);
  }
  return numCompares;
}

/************************************************************/
size_t
partition(vector<int>& aCopy, size_t first, size_t last, size_t& numCompares)
{
  median3 (aCopy, first, last - 1, numCompares);
  int pivot = aCopy[last - 2];
  
  size_t up = first;
  size_t down = last - 2;
  for ( ; ; ) {

    while (aCopy[++up] < pivot) { ++numCompares; }
    while (aCopy[--down] > pivot) { ++numCompares; }
    if (up >= down) break;
    std::swap (aCopy[up], aCopy[down]);
    ++numCompares;
  }
  std::swap (aCopy[last - 2], aCopy[up]);
  return up;
}
/************************************************************/

size_t
median3 ( vector<int> & a, int left, int right, size_t& numCompares)
{
  int center = (left + right) / 2;
  if( a[center] < a[left] )
  {
    std::swap( a[left], a[center] );
    ++numCompares;
  }
  if( a[right] < a[left] )
  {
    std::swap( a[left], a[right] );
    ++numCompares;
  }
  if( a[right] < a[center] )
  {
    std::swap( a[center], a[right] );
    ++numCompares;
  }

  std::swap( a[center], a[right - 1] );
  return a[right - 1];
}

/************************************************************/

size_t
insertionSort (vector<int>& v, size_t begin, size_t end)
{
  size_t numCompares = 0;
  for (size_t index = begin; index < end; ++index)
  {
    
    int i = index;
    int elem = v[i];
    while (i >= 1 && elem < v[i - 1])
    {
      v[i] = v[i - 1];
      --i;
      ++numCompares;
    }
    v[i] = elem;
  }
  return numCompares;
}

/************************************************************/

size_t
shellSort (vector<int>& v)
{
  size_t N = v.size();
  size_t h;
  size_t numCompares = 0;

  for (h = 1; h <= N/3; h = 3*h + 1){}
  
  while (h > 0)
  {
    for (size_t i = h; i <= N-1; i++)
    {
      size_t j = i; 
      int temp = v[i];
      while (j >= h && temp < v[j - h])
      {
        v[j] = v[j - h]; 
        j -= h;
        ++numCompares;
      }
      v[j] = temp;
    }
    h /= 3;
  }

  return numCompares;
}

/************************************************************/
/************************************************************/