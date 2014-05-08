
#include "testFactory.h"


#include "htkMultiThreader.h"



struct MyData
{
	int *data;
	int num;
	int* flag;
};
HTK_THREAD_RETURN_TYPE MultiThreadCallback(void * arg)
{
	typedef htk::MultiThreader::ThreadInfoStruct ThreadInfoType;
	ThreadInfoType * infoStruct = static_cast<ThreadInfoType*>(arg);
	const unsigned int threadID = infoStruct->ThreadID;
	const unsigned int numOfThreads = infoStruct->NumberOfThreads;
	MyData* data = static_cast<MyData*>(infoStruct->UserData);

	int* _data = data->data;
	int num = data->num;
	int* flag = data->flag;

	HTK_THREAD_RETURN_TYPE value;
	int blockSize =   static_cast<int>  ( ceil( num*1.0 / numOfThreads) ); 
	//printf("threadID num blockSize are %d %d %d\n",threadID, num,blockSize);
	
	//if(flag[threadID]) return value;

	for(int i= threadID*blockSize; i< (threadID+1) *blockSize ;i++)
	{
		
		if(i<num )
		{
			_data[i]++;
		}
	}	

	//flag[threadID] =1;
	return value;
}


#include "time.h"
int testMultiThread()
{
  int numberOfThreads = 13;
  int size = 500*500*500;
  int* data = new int[size];
  int* flag = new int[numberOfThreads];

  MyData *myData = new MyData;
  myData->data = data;
  myData->num = size;
  myData->flag = flag;
  memset(data,0,size*sizeof(int));
  memset(flag,0,numberOfThreads*sizeof(int));
  clock_t t1,t2;
  t1 = clock();
  htk::MultiThreader::Pointer threader = htk::MultiThreader::New();
  if(threader.IsNull())
    {
    return EXIT_FAILURE;
    }
  
  threader->SetNumberOfThreads(numberOfThreads);
  threader->SetSingleMethod(MultiThreadCallback,(void*)myData);
  threader->SingleMethodExecute();
  t2=clock();
  double runningTime = ( double(t2) - double(t1)) /CLOCKS_PER_SEC;
  printf("time for thread func is %f\n", runningTime);

  t1 = clock();
  for(int i=0;i<size;i++)
  {
	  data[i]++;
  }
  t2=clock();
  runningTime = ( double(t2) - double(t1)) /CLOCKS_PER_SEC;
  printf("time for non-thread func is %f\n", runningTime);


  bool resultSame=true;
  for(int i=0;i<size;i++)
  {
	 // printf("%d\n",data[i]);
	  if(data[i]!=2)
	  {
		  resultSame = false;
	  }
  }

  if(resultSame)
   printf("result same\n");
  else
  printf("result NOT same\n");

  if(data){delete [] data;}
  if(flag){delete [] flag;}
 // htk::MultiThreader::SetGlobalDefaultNumberOfThreads( numberOfThreads );
}


int main(int argc, char* argv[])
{
	testMultiThread();
	///Test MyObjectFactory  
    //std::cout << "/n-----------------------Test MyObjectFactory----------------------/n";  
	//htk::MyObjectFactoryTest(argc, argv);  
	//htk::MyHDFObjectFactoryTest(argc, argv);  

	//htkMultiThreaderTest(argc, argv);

	return 1;
}