#include <linker.h>

int main(int argc, char ** argv)
{
    if (argc!=2)
    {
        std::cerr<<"Arguments number is not valid, this program recieves the name of file to be assembled as an argument."<<std::endl;
        return 0;
    }else
    {
        std::string name(argv[1]);

        if (name.substr(name.find_last_of(".") + 1) != "s")
        {
            std::cerr<<"File extension is not valid, only accepts .s files."<<std::endl;
            return 0;
        }else
        {       
            std::cout<<"Recognized file name: "<<name<<std::endl;
            Linker linker(name.substr(0, name.size()-2));
            linker.link();   
        }
    }
    return 0;
}
