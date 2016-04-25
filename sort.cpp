/*
 * testSmartSort.cpp
 *
 * Usage: ./testSmartSort <numElements> <numThreads>
 */

#include <tsgl.h>
#include <omp.h>

using namespace tsgl;

const int MARGIN = 8;    // Border for drawing
/*!
 * \brief Visualization of the bottom-up mergesort algorithm.
 * \details Utilizes the sortData struct and sorts a number of items using the mergesort algorithm.
 * \details Uses lines to represent the items being sorted.
 * \details At the start, the items being sorted are all divided.
 * \details Once items have been sorted in one divided section, then sections are merged and the process repeats itself.
 * \details Different colors represent different sections being sorted.
 * \details Once all items have been sorted and merged, the animation stops and all lines are colored white.
 */
void smartSortFunction(Canvas& can, int threads, int size) {
  
    int* numbers = new int[size];       // Array to store the data
    //srand (time(NULL));
    for (int i = 0; i < size; i++)
      numbers[i] = rand() % (can.getWindowHeight() - MARGIN);

    #pragma omp parallel num_threads(threads)
    {   
        int tid = omp_get_thread_num();
        int cwh = can.getWindowHeight() - MARGIN/2;
        int offset = size/threads * tid;
        
        int chunksize = size/threads;
        int temp, left;
        ColorFloat cl = Colors::highContrastColor(tid);
        printf("tid = %d, offset = %d R= %f, G=%f, B = %f, A = %f\n", tid, offset, cl.A, cl.G, cl.B, cl.A);

            can.pauseDrawing();  //Tell the Canvas to stop updating the screen temporarily
            //can.drawRectangle(0, 0 , 1024 ,cwh,can.getBackgroundColor());
            for(int i=0; i<chunksize/2;i++){
              left = MARGIN/2 + (offset+i*2)*8;
              can.drawRectangle(left, cwh - numbers[offset+i*2], left+8, cwh, WHITE);
              can.drawRectangle(left+8, cwh - numbers[offset+i*2+1], left+16, cwh, WHITE);
            }
            can.resumeDrawing(); 
  
        while (can.isOpen()) {
            //even first
            can.sleep();
          
            for(int i=0; i<chunksize/2;i++){

              if(numbers[offset+ i*2]>numbers[offset+i*2+1]){
                can.pauseDrawing(); 
                temp = numbers[offset+i*2];
                numbers[offset+i*2] = numbers[offset+i*2+1];
                numbers[offset+i*2+1] = temp;
                left = MARGIN/2 + (offset+i*2)*8;
                can.drawRectangle(left, cwh - numbers[offset+i*2], left+8, cwh, cl);
                can.drawRectangle(left+8, cwh - numbers[offset+i*2+1], left+16, cwh, cl);
                can.resumeDrawing(); //Tell the Canvas it can resume updating

                can.sleep();
                //cover the color and draw new values
                can.pauseDrawing();
                can.drawRectangle(left, 0, left+8, cwh, BLACK);
                can.drawRectangle(left+8, 0, left+16, cwh, BLACK);

                can.drawRectangle(left, cwh - numbers[offset+i*2], left+8, cwh, WHITE);
                can.drawRectangle(left+8, cwh - numbers[offset+i*2+1], left+16, cwh, WHITE);

                can.resumeDrawing(); //Tell the Canvas it can resume updating
              }
            }
            //odd
            for(int i=0; i<chunksize/2;i++){

              if(numbers[offset+ i*2+1]>numbers[offset+i*2+2]){
                can.pauseDrawing(); 
                temp = numbers[offset+i*2+1];
                numbers[offset+i*2+1] = numbers[offset+i*2+2];
                numbers[offset+i*2+2] = temp;
                left = MARGIN/2 + (offset+i*2+1)*8;
                can.drawRectangle(left, cwh - numbers[offset+i*2+1], left+8, cwh, cl);
                can.drawRectangle(left+8, cwh - numbers[offset+i*2+2], left+16, cwh, cl);
                can.resumeDrawing(); //Tell the Canvas it can resume updating

                can.sleep();
                //cover the color and draw new values
                can.pauseDrawing();
                can.drawRectangle(left, 0, left+8, cwh, BLACK);
                can.drawRectangle(left+8, 0, left+16, cwh, BLACK);
                can.resumeDrawing(); //Tell the Canvas it can resume updating

                can.drawRectangle(left, cwh - numbers[offset+i*2+1], left+8, cwh, WHITE);
                can.drawRectangle(left+8, cwh - numbers[offset+i*2+2], left+16, cwh, WHITE);
             
                  can.resumeDrawing(); //Tell the Canvas it can resume updating
              }
            }
        }
    }
  delete [] numbers;


}

//Takes in command line arguments for the window width and height
//as well as for the number of threads to use
int main(int argc, char* argv[]) {
    int s = (argc > 1) ? atoi(argv[1]) : 128;
    if (s < 10) s = 10;
    int w = 1024 + MARGIN;
    int h = w/2;

    int threads, t = (argc > 2) ? atoi(argv[2]) : omp_get_num_procs();
    for (threads = 1; threads < t; threads *=2);  //Force threads to be a power of 2

    Canvas c(-1, -1, w, h, "Bubble Sort", FRAME);
    c.setBackgroundColor(BLACK);
    c.run(smartSortFunction, threads, s);
}
