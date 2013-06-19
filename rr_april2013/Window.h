#ifndef Window_h
#define Window_h

#define WINDOW_SIZE 4

struct Window
{
  Window() { 
    total = cur = 0; 
    for (int i=0; i<WINDOW_SIZE; ++i) 
      values[i] = 0;
  }
  
  // Add a new sample to the window
  void add(int value) {
    if (cur == WINDOW_SIZE) cur = 0;
    long subt = total - values[cur];
    values[cur++] = value;
    total = subt + value;
  }

  // Return the current average as a float
  float avgF() { return total * ooWS; }
  
  // Return the current average as an integer
  int avg() { return total / WINDOW_SIZE; }
  
private:
  int values[WINDOW_SIZE];      // the last X values
  int cur;                      // the index of the last value
  volatile long total;          // the rolling total
  
  static const float ooWS = 1.f / WINDOW_SIZE;
};

#endif//Window_h

