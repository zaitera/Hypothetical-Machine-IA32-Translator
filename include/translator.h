#ifndef TRANSLATOR_H
#define TRANSLATOR_H
#include <preprocessor.h>

class Translator
{
private:
    //! A private pointer to a file.
    /*! points to the file that contains the source code to be processed. */
    std::fstream* source_code_file;
    //! Private atribute,
    /*!the file being assembled in our representation.*/
    TupleList file_being_assembled;
    const std::map<std::string, uint8_t> mem_spaces_MP = {
        {"ADD", 2},
        {"SUB", 2},
        {"MULT", 2},
        {"DIV", 2},
        {"JMP", 2},
        {"JMPN", 2},
        {"JMPP", 2},
        {"JMPZ", 2},
        {"COPY", 3},
        {"LOAD", 2},
        {"STORE", 2},
        {"INPUT", 2},
        {"OUTPUT", 2},
        {"C_INPUT", 2},
        {"C_OUTPUT", 2},
        {"H_INPUT", 2},
        {"H_OUTPUT", 2},
        {"S_INPUT", 3},
        {"S_OUTPUT", 3},
        {"STOP", 1},
    };   
public:
    std::string file_name;
    //! Class constructor, receives a pointer to the source file.
    /*!
        constructs the object and indicates the file to be processed.
    */
    Translator(std::fstream *);

    //! Class destructor,
    /*!
        destructs the object guarantees a clean exit for the program.
    */
    ~Translator();

    //! Public method
    /*!
        This method does the preprocessing action from a to z for the defined source code.
    */
    void translate(void);
};
#endif /* TRANSLATOR_H */