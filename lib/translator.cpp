#include <translator.h>

Translator::Translator(std::fstream *source)
{
    this->source_code_file = source;
}

Translator::~Translator(){}


void Translator::translate(void){}