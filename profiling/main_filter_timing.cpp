#include <chrono>
#include <iostream>
#include <vector>

#include "../src/util/plotting/plot.h"
#include "../src/util/radarDataTypes.h"
#include "../src/signal_processing/tuneFilter.h"
#include "../src/waveform/LFM.h"
namespace plt = matplotlibcpp;

int main(){
  //HACK hard coding the taps, can add some ability to parse a file or somthing
  std::vector<float> taps = {0.0008302388596348464, 0.000955868570599705, -0.0020353347063064575, 3.3073416789746066e-18, 0.004050636198371649, -0.0035107277799397707, 
    -0.004825763404369354, 0.010514914989471436, -1.0744886732703555e-17, -0.01813529059290886, 0.014474445022642612, 0.018609102815389633, -0.03878936544060707, 
    1.9515073313200094e-17, 0.06743156164884567, -0.058430444449186325, -0.09081077575683594, 0.3001424968242645, 0.5990568399429321, 0.3001424968242645, 
    -0.09081077575683594, -0.058430444449186325, 0.06743156164884567, 1.9515073313200094e-17, -0.03878936544060707, 0.018609102815389633, 0.014474445022642612, 
    -0.01813529059290886, -1.0744886732703555e-17, 0.010514914989471436, -0.004825763404369354, -0.0035107277799397707, 0.004050636198371649, 3.3073416789746066e-18, 
    -0.0020353347063064575, 0.000955868570599705, 0.0008302388596348464};

  int numIter = 1000;
  std::vector<float> data(numIter,0);
  std::vector<float> data2(numIter,0);
  
  {
    //filter object
    tuneFilter* filt = new tuneFilter(taps,5000000, 10000000.0, 10000);

    //data
    LFM* waveGen = new LFM(10000000.0, //sample rate
		      (int) 10000,                      //number of samples
		      (float) 50000,                    //bandwidth
		      (float) 100000);                  //center frequency
    waveGen->genWave();
    radar::complexFloatBuffPtr tx_wave = waveGen->getFloatBuff();
  
    
    //run tests
    for(int i = 0;i<numIter;i++){
      auto start = std::chrono::system_clock::now();
      filt->timeFilterTune(tx_wave.get(),tx_wave.get());
      auto duration = std::chrono::duration_cast<
      std::chrono::duration<float> >(std::chrono::system_clock::now() - start);
      data[i] = (duration.count());
    }
    
    delete filt;
    delete waveGen;
    
  }
  
  
  {
    //filter object
    tuneFilter* filt = new tuneFilter(taps,5000000, 10000000.0, 100000);

    //data
    LFM* waveGen = new LFM(10000000.0, //sample rate
		      (int) 100000,                      //number of samples
		      (float) 50000,                    //bandwidth
		      (float) 100000);                  //center frequency
    waveGen->genWave();
    radar::complexFloatBuffPtr tx_wave = waveGen->getFloatBuff();
  
    
    //run tests
    for(int i = 0;i<numIter;i++){
      auto start = std::chrono::system_clock::now();
      filt->timeFilterTune(tx_wave.get(),tx_wave.get());
      auto duration = std::chrono::duration_cast<
      std::chrono::duration<float> >(std::chrono::system_clock::now() - start);
      data2[i] = (duration.count());
    }
    
    delete filt;
    delete waveGen;
    
  }

  
  
    plt::figure();
    plt::xlabel("interation");
    plt::ylabel("time (s)");
    plt::title("Filter run time");
    plt::plot(data,"-b*");
    plt::plot(data2,"-g*");
    plt::show();
  
  

}