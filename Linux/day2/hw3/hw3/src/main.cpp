#include <iostream>
#include "hw3.hpp"
using namespace KMJ;
Monster::Monster(int x, int y, int HP) : x(x), y(y), HP(HP) {}
Player::Player(int x, int y) : x(x), y(y), HP(50), MP(10) {}

Player player(0, 0);
Monster monster(5, 4, 50);

int Monster::Be_Attacked(){
    HP -= 10;
    return HP;
}

void Player::X_move(int move){
    player.x += move;
}

void Player::Y_move(int move){
    player.y += move;
}

void Player::Attack(Monster &target){
    target.Be_Attacked();
}

int main(){
    player.MP = 10;
    player.HP = 50;
    int monster_attack = 0;
    while(1){
        if(monster.HP == 0 && player.HP == 0){
            std::cout<<"같이 쓰러졌습니다.."<<std::endl;
            return 0;
        }
        else if(monster.HP == 0){
            std::cout<<"몬스터가 쓰러졌습니다!"<<std::endl;
            return 0;
        }
        else if(player.MP == 0){
            std::cout<<"MP부족!"<<std::endl;
            return 0;
        }
        else if(player.HP == 0){
            std::cout<<"플레이어가 쓰러졌습니다..."<<std::endl;
            return 0;
        }
        char choice;
        std::cout<<"Type Command (A/U/D/R/L/S)"<<std::endl;
        std::cin>>choice;
        if(choice == 'A'){
            player.MP --;
            if(player.x == 5 && player.y == 4){
                player.Attack(monster);
                std::cout<<"공격 성공!"<<std::endl<<"몬스터 체력: "<<monster.HP<<std::endl;
                monster_attack++;
                if(monster_attack > 2){
                    player.HP -= 25;
                    std::cout<<"공격 당함!"<<std::endl<<"플레이어 체력: "<<player.HP<<std::endl;
                }
            }
            else{
                std::cout<<"공격 실패!"<<std::endl;
                monster_attack = 0;
            }
        }
        else if(choice == 'U'){
            monster_attack = 0;
            int move = 1;
            player.Y_move(move);
            std::cout<<"위로 1 올라감!"<<std::endl;
        }
        else if(choice == 'D'){
            monster_attack = 0;
            int move = -1;
            player.Y_move(move);
            std::cout<<"아래로 1 내려감!"<<std::endl;
        }
        else if(choice == 'R'){
            monster_attack = 0;
            int move = 1;
            player.X_move(move);
            std::cout<<"오른쪽으로 1 이동!"<<std::endl;
        }
        else if(choice == 'L'){
            monster_attack = 0;
            int move = -1;
            player.X_move(move);
            std::cout<<"왼쪽으로 1 이동!"<<std::endl;
        }
        else if(choice == 'S'){
            std::cout<<"HP: "<<player.HP<<std::endl<<"MP :"<<player.MP
            <<std::endl<<"Position: "<<player.x<<", "<<player.y<<std::endl;
        }
        else{
            std::cout<<"잘못 입력했습니다."<<std::endl;
        }
    }
}