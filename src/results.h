class Result{
    private:
        bool result;
    public:
        Result(){
            this -> result = false;
        }
        Result(bool result){
            this -> result = result;
        }
        int getResult(){
            //-1 indicates exit, 0 indicates failed command, 1 indicates successful command
            if(this -> result){
                return 1;
            }
            else{
                return 0;
            }
        }
};

class ExitResult : public Result{
    public:
        ExitResult(){}
        int getResult(){
            return -1;
        }
};