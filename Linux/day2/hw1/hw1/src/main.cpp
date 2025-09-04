#include <iostream>
#include "hw1.hpp"
using namespace KMJ;

int main(){
    homework CAL;
    if(CAL.number() == 0){
        return 0;
    }
    CAL.MAX();
    CAL.MIN();
    CAL.SUM();
    CAL.AVG();
}