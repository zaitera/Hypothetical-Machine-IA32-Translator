#include <translator.h>

constexpr unsigned long long int HashStringToInt(const char *str, unsigned long long int hash = 0)
{
    return (*str == 0) ? hash : 101 * HashStringToInt(str + 1) + *str;
}


Translator::Translator(std::fstream *source)
{
    this->source_code_file = source;
}

Translator::~Translator(){
}


void Translator::translate(void){
    
    try{
        PreProcessor pre_processor(this->source_code_file);
        pre_processor.file_name = this->file_name;
        this->file_being_translated = pre_processor.preProcess();
    } catch(std::string errmsg)
    {
        std::cout<<errmsg<<std::endl;
    }
    std::string str("test");
    switch (HashStringToInt(str.c_str()))
    {
        case HashStringToInt("first"):
            std::cout << "first case" << std::endl;
            break;
        case HashStringToInt("second"):
            std::cout << "second case" << std::endl;
            break;
        default:
            break;
    }
}