namespace KMJ{
class Monster{
    public:
    int HP, x, y;
    Monster();
    Monster(int x, int y, int HP);
    int Be_Attacked();
};

class Player{
    public:
    int HP, MP, x, y;
    Player();
    Player(int x, int y);
    void Attack(Monster &target);
    void Show_Status();
    void X_move(int move);
    void Y_move(int move);
};
}

/*
A - 공격
U - 위로 y축 +1
D - 아래로 y축 -1
R - 오른쪽 x축 +1
L - 왼쪽 x축 -1
S - 상태 HP, MP, 위치 나타내기
*/