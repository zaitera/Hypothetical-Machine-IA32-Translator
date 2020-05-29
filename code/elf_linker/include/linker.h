#ifndef LINKER_H
#define LINKER_H

#include <iostream>
#include <fstream>
#include <elfio/elfio.hpp> 

using namespace ELFIO;

class Linker
{
    std::string file_name;
    public:
        //! Class constructor, receives a pointer to the source file.
        /*!
            constructs the object and indicates the file to be processed.
        */
        Linker(std::string);

        //! Class destructor,
        /*!
            destructs the object guarantees a clean exit for the program.
        */
        ~Linker();

        //! Public method
        /*!
            This method does the preprocessing action from a to z for the defined source code.
        */
        void link(void);
};
#endif /* LINKER_H */
