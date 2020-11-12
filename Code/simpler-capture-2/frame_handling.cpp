FrameQueue initQueue(int size) {
   FrameQueue theQueue;
   theQueue.frames = malloc(size * sizeof(IplImage*));
   theQueue.nextFrameIndex = 0;
   theQueue.numberOfFrames = 0;
   if(theQueue.frames != null) {
      theQueue.maxSize = size;
    } else {
       theQueue.maxsize = 0;
    }
    return theQueue;
}
int enqueue(FrameQueue* queue, IplImage* frame);
IplImage* dequeue(FrameQueue* queue);

void captureFrame()
void writebackFrame(int frameNum, )
