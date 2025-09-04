#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "hw2.hpp"
using namespace KMJ;

int main(){
    homework2 HI;
    coor *point;
    int count, range_min, range_max, index1, index2, index3, index4;
    std::cout<<"점의 개수: ";
    std::cin>>count;
    if (count < 2){
        std::cout<<"제대로 입력해주세요."<<std::endl;
        return 0;
    }
    point = new coor[count];
    std::cout<<"좌표 최소 범위: ";
    std::cin>>range_min;
    if(std::cin.fail()){
                std::cout<<"정수를 제대로 입력해주세요."<<std::endl;
                return 0;
            }
    std::cout<<"좌표 최대 범위: ";
    std::cin>>range_max;
    if(std::cin.fail()){
                std::cout<<"정수를 제대로 입력해주세요."<<std::endl;
                return 0;
            }
    srand((unsigned)time(NULL));
    for(int i = 0; i < count; i++){
        point[i].x = range_min + rand() % (range_max - range_min + 1);
        point[i].y = range_min + rand() % (range_max - range_min + 1);
    }
    for(int i = 0; i < count; i++){
        std::cout<<"point "<<i+1<< ". X = "<<point[i].x <<" Y = "<<point[i].y<<std::endl;
    }
    double max = HI.CAL(point[0].x, point[0].y,point[1].x,point[1].y);
    double min = HI.CAL(point[0].x, point[0].y,point[1].x,point[1].y);
    for(int i = 0; i< count;i++){
        for(int j = 0; j<count;j++){
            if(i==j){
                continue;
            }
            double temp = HI.CAL(point[i].x, point[i].y,point[j].x,point[j].y);
            if(max < temp){
                max = temp;
                index3 = i;
                index4 = j;
            }
            if(min > temp){
                min = temp;
                index1 = i;
                index2 = j;
            }
        }
    }
    std::cout<<"최솟값: "<<min<<"\n"<<"최솟값인 두 점: "<<"P1: <"<<point[index1].x<<", "<<point[index1].y
    <<"> "<<"P2: <"<<point[index2].x<<", "<<point[index2].y<<">"<<std::endl;
    std::cout<<"최댓값: "<<max<<"\n"<<"최댓값인 두 점: "<<"P1: <"<<point[index3].x<<", "<<point[index3].y
    <<"> "<<"P2: <"<<point[index4].x<<", "<<point[index4].y<<">"<<std::endl;
    delete [] point;
}