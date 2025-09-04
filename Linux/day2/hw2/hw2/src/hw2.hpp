namespace KMJ{
class homework2{
    private:

    public:
    double CAL(int x1, int y1, int x2, int y2){
        double x = 2;
        double sum1 = x1 - x2;
        double sum2 = y1 - y2;
        double sum3 = sum1 * sum1 + sum2 * sum2;
        for (int i = 0; i < 10; i++){
            x = (x + (sum3 / x)) / 2;
        }
        return x;
    }
};


struct coor{
    int x, y;
};
}