#ifndef FILTER_IIR_H
#define FILTER_IIR_H

#include "Queue.hpp"

//fdatool coff

//IIR滤波 树状滤波(?)
const int NL = 7;
const double NUM[7] = {
  8.647182682702e-05,                 0,-0.0002594154804811,                 0,
  0.0002594154804811,                 0,-8.647182682702e-05
};
const int DL = 7;
const double DEN[7] = {
                   1,   -5.816988323661,    14.10212014572,    -18.2377974322,
      13.27055943226,   -5.151265517789,   0.8333716956989
};

template<typename T>
class Filter_IIR
{
public:
    Filter_IIR();
    ~Filter_IIR();
    char filterData(T data,T* res);
private:
    unsigned int orders;
    Queue<T>* num;
    Queue<T>* den;

};

template<typename T>
Filter_IIR<T>::Filter_IIR(){
    orders = DL-1;  // 6阶！！！！！！
    num = new Queue<T>(DL);
    den = new Queue<T>(DL);
    for(int i = 0;i< orders; i ++){
        num->push(0);
        den->push(0);
    }

}

template<typename T>
Filter_IIR<T>::~Filter_IIR(){
    delete num;
    delete den;
}

template<typename T>
char Filter_IIR<T>::filterData(T data,T *res){
    unsigned int n = orders;
    T temp,temp_res;
    temp_res = data*NUM[0];
           while(n--){         //n-1 ~0
               num->at(n,temp);   // 新 -> 老
              temp_res += temp*NUM[DL-1-n];  // 此处已经减过1
               den->at(n,temp);
              temp_res -= temp*DEN[DL-1-n];
          }
    num->pop(temp);  //必须先pop
    den->pop(temp);
    num->push(data);
    den->push(temp_res);

    *res = temp_res;
    return 1;
}





#endif // FILTER_H
