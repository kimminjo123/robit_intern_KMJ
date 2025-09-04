namespace KMJ{
class homework{
    private:
    int sum = 0;
    int many = 0;
    float avg = 0;
    int *n;
    public:
    int number(){
        std::cout<<"몇 개의 원소를 할당하겠습니까? : ";
        std::cin>>many;
        if(many <= 0){
            std::cout<<"숫자를 잘못 입력하였습니다."<<std::endl;
            return 0;
        }
        n = new int[many];
        for(int i = 0; i < many; i++){
            std::cout<<"정수형 데이터 입력: ";
            std::cin>>n[i];
            if(std::cin.fail()){
                std::cout<<"정수형 데이터를 잘못 입력하였습니다."<<std::endl;
                return 0;
            }
        }
        return 1;
    }
    void SUM(){
        for(int i = 0; i < many; i++){
            sum += n[i];
        }
        std::cout<<"전체합: "<<sum<<std::endl;
    };
    void MAX(){
        int max = n[0];
        for(int i = 0; i < many; i++){
            if(max < n[i]){
                max = n[i];
            }
        }
        std::cout<<"최대값: "<<max<<std::endl;
    };
    void MIN(){
        int min = n[0];
        for(int i = 0; i < many; i++){
            if(min > n[i]){
                min = n[i];
            }
        }
        std::cout<<"최소값: "<<min<<std::endl;
    };
    void AVG(){
        avg = (float)sum / (float)many;
        std::cout<<"평균: "<<avg<<std::endl;
        delete [] n;
    };
};
}