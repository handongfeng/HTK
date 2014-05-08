//#ifndef __htkMultiThreaderTest_h
//#define __htkMultiThreaderTest_h

#include "htkMultiThreader.h"

bool VerifyRange(int value, int min, int max, const char * msg)
{
  if( value < min )
    {
    std::cerr << msg << std::endl;
    return false;
    }

  if( value > max )
    {
    std::cerr << msg << std::endl;
    return false;
    }
  return true;
}



bool SetAndVerifyGlobalMaximumNumberOfThreads( int value )
{
  htk::MultiThreader::SetGlobalMaximumNumberOfThreads( value );
  return VerifyRange( htk::MultiThreader::GetGlobalMaximumNumberOfThreads(),
        1, HTK_MAX_THREADS, "Range error in MaximumNumberOfThreads");
}

bool SetAndVerifyGlobalDefaultNumberOfThreads( int value )
{
  htk::MultiThreader::SetGlobalDefaultNumberOfThreads( value );
  return VerifyRange( htk::MultiThreader::GetGlobalDefaultNumberOfThreads(),
        1, htk::MultiThreader::GetGlobalMaximumNumberOfThreads(),
        "Range error in DefaultNumberOfThreads");
}

bool SetAndVerifyNumberOfThreads( int value, htk::MultiThreader * threader )
{
  threader->SetNumberOfThreads( value );
  return VerifyRange( threader->GetNumberOfThreads(),
        1, htk::MultiThreader::GetGlobalMaximumNumberOfThreads(),
        "Range error in NumberOfThreads");
}

int htkMultiThreaderTest(int argc, char* argv[])
{
  // Choose a number of threads.
  int numberOfThreads = 10;
  if( argc > 1 )
    {
    const int nt = atoi( argv[1] );
    if(nt > 1)
      {
      numberOfThreads = nt;
      }
    }

  htk::MultiThreader::Pointer    threader = htk::MultiThreader::New();
  if(threader.IsNull())
    {
    return EXIT_FAILURE;
    }

  htk::MultiThreader::SetGlobalDefaultNumberOfThreads( numberOfThreads );

  {
  // Test settings for GlobalMaximumNumberOfThreads

  bool result = true;

  result &= SetAndVerifyGlobalMaximumNumberOfThreads( -1 );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  0 );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  1 );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  2 );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  HTK_MAX_THREADS  );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  HTK_MAX_THREADS - 1 );
  result &= SetAndVerifyGlobalMaximumNumberOfThreads(  HTK_MAX_THREADS + 1 );

  if( !result )
    {
    return EXIT_FAILURE;
    }


  result &= SetAndVerifyGlobalDefaultNumberOfThreads( -1 );
  result &= SetAndVerifyGlobalDefaultNumberOfThreads(  0 );
  result &= SetAndVerifyGlobalDefaultNumberOfThreads(  1 );
  result &= SetAndVerifyGlobalDefaultNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
  result &= SetAndVerifyGlobalDefaultNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads() - 1 );
  result &= SetAndVerifyGlobalDefaultNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads() + 1 );

  if( !result )
    {
    return EXIT_FAILURE;
    }

  htk::MultiThreader::Pointer threader2 = htk::MultiThreader::New();

  result &= SetAndVerifyNumberOfThreads( -1, threader2 );
  result &= SetAndVerifyNumberOfThreads(  0, threader2 );
  result &= SetAndVerifyNumberOfThreads(  1, threader2 );
  result &= SetAndVerifyNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads(), threader2 );
  result &= SetAndVerifyNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads() - 1, threader2);
  result &= SetAndVerifyNumberOfThreads( htk::MultiThreader::GetGlobalMaximumNumberOfThreads() + 1, threader2);

  if( !result )
    {
    return EXIT_FAILURE;
    }

  }

  return EXIT_SUCCESS;
}

//namespace htkMultiThreaderTestHelpers
//{
//
//void ThreadedMethod()
//{
//#ifdef HTK_USE_PTHREADS
//// ThreadProcessIDType threadId = pthread_self();
//#endif
//
//#ifdef HTK_USE_WIN32_THREADS
//// ThreadProcessIDType threadId = GetCurrentThread();
//#endif
//}
//
//} // end of htkMultiThreaderTestHelpers


//#endif