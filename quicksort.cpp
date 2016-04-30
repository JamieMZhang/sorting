/*
 * testSmartSort.cpp
 *
 * Usage: ./testSmartSort <numElements> <numThreads>
 */

#include <iostream>
 #include <tsgl.h>
#include <omp.h>
#include <cmath>
#include <unistd.h>
using namespace tsgl;

const int MARGIN = 8;  
int k=0;
int cwh;
int size;
int* numbers;
int width = 8;
int delay = 100;

void busyWait(){
    for(int i = 0; i<delay; i++)
        for(int j =0;j<delay;j++)
            for(int k =0; k<delay;k++){
            int temp = sqrt(i+j+k);
            }
        
}

int partition(Canvas& can, int arr[], int low_index, int high_index)
{
    
    int i, j, temp, key;
    key = arr[low_index];
    j= high_index;
    i= low_index + 1;

    int left=MARGIN/2+low_index*width;
    int right;
    //printf("%d ", omp_get_thread_num());

    can.drawRectangle(left, cwh - arr[low_index], left+width, cwh, GREEN);
    printf("ID = %d\n", omp_get_thread_num());
    
    while(1)
    {
        while(i < high_index && key >= arr[i])
            {   
                can.sleep();
                left = MARGIN/2+i*width;
                can.drawRectangle(left, cwh - arr[i], left+width, cwh, Colors::highContrastColor(omp_get_thread_num()));
                busyWait();
                i++;
            }
            
        while(key < arr[j])
            {

                can.sleep();
                right = MARGIN/2 + (j)*width;
                can.drawRectangle(right, cwh - arr[j], right+width, cwh, Colors::highContrastColor(omp_get_thread_num()));
                busyWait();
                j--;
            }
        if(i < j)
        {
            temp = arr[i];
            arr[i] = arr[j];
            left = MARGIN/2+i*width;
            can.drawRectangle(left, 0, left+width, cwh, BLACK);
            can.drawRectangle(left, cwh - arr[i], left+width, cwh, Colors::highContrastColor(omp_get_thread_num()));

            arr[j] = temp;
            right = MARGIN/2 + (j)*width;
            can.drawRectangle(right, 0, right+width, cwh, BLACK);
            can.drawRectangle(right, cwh - arr[j], right+width, cwh, Colors::highContrastColor(omp_get_thread_num()));

        }
        else
        { //place pivot point
            temp= arr[low_index];
            left=MARGIN/2+low_index*width;
            arr[low_index] = arr[j];
            can.drawRectangle(left, 0, left+width, cwh, BLACK);
            can.drawRectangle(left, cwh - arr[low_index], left+width, cwh,Colors::highContrastColor(omp_get_thread_num()));
            arr[j]= temp;
            right = MARGIN/2 + (j)*width;
            can.drawRectangle(right, 0, right+width, cwh, BLACK);
            can.drawRectangle(right, cwh - arr[j], right+width, cwh, GREEN);
            //fade
            can.drawRectangle(low_index*width+MARGIN/2, 0, (high_index+1)*width+MARGIN/2, cwh, ColorInt(0, 0, 0, 150));//
            //busyWait();

            return(j);
        }
    }
}


void quicksort(Canvas& can, int arr[], int low_index, int high_index)
{
    int j;
    
    //#pragma omp shared(arr) 
    if(low_index < high_index)
    {
        //j = partition(arr, low_index, high_index);
        omp_set_num_threads(8);

        int i, temp=0, key;
        bool flag = true;
        key = arr[low_index];
        i= low_index + 1;
        j= high_index;
        i= low_index + 1;
        j=partition(can, arr, low_index, high_index);
        //printf("id= %d, key = %d, temp = %d, low = %d, high = %d, i = %d, j=%d\n", omp_get_thread_num(), key, temp, low_index, high_index, i,j);

        {
        //#pragma omp parallel sections
        {
        //printf ("total threads  = %d, \n", omp_get_num_threads()); 
       // #pragma omp section
            {
                quicksort(can, arr, low_index, j - 1); 
            }
        //#pragma omp section
            { 
               
                quicksort(can, arr, j + 1, high_index);
            }
        }
            


        }
    }
}


void smartSortFunction(Canvas& can, int threads, int size) {
  
    numbers = new int[size];       // Array to store the data
    //srand (time(NULL));
    for (int i = 0; i < size; i++)
      numbers[i] = rand()%512;

        int tid = omp_get_thread_num();
        cwh = can.getWindowHeight() - MARGIN/2;
        //int offset = size/threads * tid;
        
        int chunksize = size;
        int temp, left;
        ColorFloat cl = Colors::highContrastColor(tid);
        //printf("tid = %d, offset = %d R= %f, G=%f, B = %f, A = %f\n", tid, offset, cl.A, cl.G, cl.B, cl.A);

            can.pauseDrawing();  //Tell the Canvas to stop updating the screen temporarily
            //can.drawRectangle(0, 0 , 1024 ,cwh,can.getBackgroundColor());
            for(int i=0; i<chunksize/2;i++){
              left = MARGIN/2 + (i*2)*width;
              can.drawRectangle(left, cwh - numbers[i*2], left+width, cwh, WHITE);
              can.drawRectangle(left+width, cwh - numbers[i*2+1], left+width*2, cwh, WHITE);
            }
            can.resumeDrawing(); 
  
        //while (can.isOpen()) {
             int N = size;
             int l=0;
             int h = N-1;
      //      
             omp_set_dynamic(0);
             //omp_set_nested(1);
             //omp_set_num_threads(threads);
      //       printf("\n\n\n");


             int j;

             j=partition(can, numbers, 0, N-1);
        printf("Pivot element with index %d has been found out by thread %d\n",j,k);
        #pragma omp parallel shared(numbers)
            {
         #pragma omp sections
                {
        //printf ("total threads  = %d, \n", omp_get_num_threads()); 
            #pragma omp section
                    {
                        int j1;
                        j1=partition(can, numbers, 0, j-1);
                        {
                            {
                #pragma omp task
                                {
                                    quicksort(can, numbers, 0, j1 - 1); 
                                }
                #pragma omp task
                                { 
                                    quicksort(can, numbers, j1 + 1, j-1);
                                }
                            }
                        } 
                    }
        #pragma omp section
                    { 
                        int j2;

                        j2=partition(can, numbers, j+1, N-1);
                //printf("Pivot element with index %d has been found out by thread %d\n",j,k);
                //#pragma omp parallel shared(arr)
                        {
                 //#pragma omp sections
                            {
                //printf ("total threads  = %d, \n", omp_get_num_threads()); 
                #pragma omp task
                                {
                                    quicksort(can, numbers, j+1, j2 - 1); 
                                }
                #pragma omp task
                                { 
                                    quicksort(can, numbers, j2 + 1, N-1);
                                }
                            }
                        } 
                    }
                }
            }
             
        //     //can.sleep();
           printf("Elements of array after sorting \n");
           bool sorted = true;
                for(int i=0;i<size;i++)
                {   if(i>0 && numbers[i]<numbers[i-1])sorted=false;
                     printf("%d\t",numbers[i]);
                }
            if(sorted)
            printf("sorted\n",sorted);
        //   //delete [] numbers;
    }
        
   


int main(int argc, char* argv[])
{
    int s = (argc > 1) ? atoi(argv[1]) : 128;
    if (s < 10) s = 10;
    int w = 1024 + MARGIN;
    int h = w/2;

    int threads, t = (argc > 2) ? atoi(argv[2]) : omp_get_num_procs();
    for (threads = 1; threads < t; threads *=2);  //Force threads to be a power of 2

    Canvas c(-1, -1, w, h, "Parallel Quicksort (press ↑ to spped up, ↓ to slow down)", FRAME);

    c.bindToButton(TSGL_DOWN, TSGL_PRESS, []() {
          if(delay<1200)delay+=50;
        });
    c.bindToButton(TSGL_UP, TSGL_PRESS, []() {
          if(delay>=50)delay-=50;
        });

    c.setBackgroundColor(BLACK);
    c.run(smartSortFunction, threads, s);


    

    // printf("\n");
}

