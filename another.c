#include <iostream>  
#include <pthread.h>  
#include <stdio.h>  
#include <unistd.h>  
#include <errno.h>  
#include <stdlib.h> 
#include <semaphore.h> 


int goods[10];  
int index = -1;
int consumed = 0;
using namespace std;
sem_t mutex,empty,full,consumedNum;
void push()  
{  
  
    if(index<10)
    {
        goods[++index] = 1;
        cout<<"goods"<<endl;
        for(int i=0;i<=index;i++)
            cout<<goods[i];
        cout<<endl;
    }
    else  
        cout << "goods满了！"<< endl;
}  
void pop()  
{  
    int s=-1;  
  
    if(index>=0)  
    {
        index--;
        cout<<"goods"<<endl;
        for(int i=0;i<=index;i++)
            cout<<goods[i];
        cout<<endl;
    }
    else  
        cout << "goods空了!" << endl;
}  
  
void* Consumer(void *arg)  
{
    int* consumerNum = (int *)arg;
    while(consumed<50)  
    {  
        sem_wait(&full);
        sem_wait(&consumedNum);
        sem_wait(&mutex);
        pop();
        consumed++;
        cout << *(consumerNum)<<" Consumed a good " << endl;  
        cout<<consumed<<endl;
        cout<<"...................................................."<<endl;
        sem_post(&mutex);
        sem_post(&empty);
        sleep(1);  
    }  
}  
  
void* Producer(void *arg)  
{
    int* producerNum = (int *)arg;
    while(consumed<50)  
    {  
        sem_wait(&empty);
        sem_wait(&mutex);
        push();  
        cout << *(producerNum)<<" Produced a good " << endl;  
        cout<<consumed<<endl;
        cout<<"...................................................."<<endl;
        sem_post(&mutex);
        sem_post(&full);  
        sleep(1);  
    }  
}

int main()  
{  
    sem_init(&mutex,1,1);
    sem_init(&empty,0,10);
    sem_init(&full,0,0);
    sem_init(&consumedNum,0,50);
    pthread_t consumer_pid1[2],producer_pid2[3];  
    int error1[2],error2[3];  
    error1[0] = 0;  
    error1[1] = 0;  
    error2[0] = 0; 
    error2[1] = 0; 
    error2[2] = 0; 
    int producer1 = 1;
    int producer2 = 2;
    int producer3 = 3;
    int consumer1 = 1;
    int consumer2 = 2;
    error2[0]=pthread_create(&producer_pid2[0],NULL,Producer,&producer1);  
    error2[1]=pthread_create(&producer_pid2[1],NULL,Producer,&producer2);  
    error2[2]=pthread_create(&producer_pid2[2],NULL,Producer,&producer3);  
    error1[0]=pthread_create(&consumer_pid1[0],NULL,Consumer,&consumer1);  
    error1[1]=pthread_create(&consumer_pid1[1],NULL,Consumer,&consumer2);  
    if(error1[0]!=0 || error1[1]!=0 || error2[0]!=0 || error2[1]!=0 || error2[2]!=0)  
    {  
        printf("pthread_cread failed");  
        return 0;  
    }  
    pthread_join(consumer_pid1[0],NULL);  
    pthread_join(consumer_pid1[1],NULL);  
    pthread_join(producer_pid2[0],NULL);  
    pthread_join(producer_pid2[1],NULL);  
    pthread_join(producer_pid2[2],NULL);  
    return 0;  
}
