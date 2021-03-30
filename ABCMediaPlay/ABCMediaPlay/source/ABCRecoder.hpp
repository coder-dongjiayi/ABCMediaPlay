//
//  ABCRecoder.hpp
//  ABCMediaPlay
//
//  Created by 董家祎 on 2021/3/30.
//

#ifndef ABCRecoder_hpp
#define ABCRecoder_hpp

#include <stdio.h>
class ABCRecoder{
  
private:
    bool isStop;
public:
    void startRecoder();
    void stopRecoder();
    ABCRecoder();
    ~ABCRecoder();
};

#endif /* ABCRecoder_hpp */
