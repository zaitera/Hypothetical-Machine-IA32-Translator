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

void Translator::solveDisplacement(std::vector<std::string> line, uint16_t current_token, std::string metadata)
{
    bool used = false;
    if(line.size()-current_token>3 && line[current_token+2] == "+"){
        used = true;
        this->output_file << "push esi\n";
        this->output_file << "mov esi, "+ line[current_token+3]+"\n";
        this->output_file << metadata <<" + " << "esi*4";
    };       
    if (!used)
    {
        this->output_file << metadata;
    }
    if(line[current_token].substr(0, 3) != "JMP")
        this->output_file << "]";
    this->output_file << "\n";
    if (used)
    {
        this->output_file << "pop esi\n";
    }
    
}

void Translator::translate(void)
{
    
    try{
        PreProcessor pre_processor(this->source_code_file);
        pre_processor.file_name = this->file_name;
        this->file_being_translated = pre_processor.preProcess();
    } catch(std::string errmsg)
    {
        std::cout<<errmsg<<std::endl;
    }
    std::string ia32_filename = this->file_name.substr(0,this->file_name.find_last_of('.'))+".s";
    this->output_file.open(ia32_filename,std::ios::out);
    this->output_file<<"global _start\n";
    uint16_t current_token;
    Section section = UNDEFINED;
    for(size_t i = 0; i != this->file_being_translated.size(); i++ )
    {
        auto line = std::get<1>(this->file_being_translated[i]);
        current_token = 0;

        if (line[0] == "SECTION")
        {
            if(line[1] == "TEXT")
            {
                section = TEXT;
                this->output_file<<".text\n";
                this->output_file<<"__start:\n";                
            }else if(line[1] == "DATA") 
            {
                section = DATA;
            }
            continue;
        }
        if(line.size()>=2 && line[1] == ":")
        {
            this->output_file<<line[0]+":";
            current_token += 2;
        } 
        if(line.size()==current_token){
            this->output_file<<"\n";
            continue;
        }
        std::string aux;
        switch (section)
        {
            case TEXT:
                switch (HashStringToInt(line[current_token].c_str()))
                {
                    case HashStringToInt("ADD"):
                    case HashStringToInt("SUB"):
                        aux = "   ";
                        std::transform(line[current_token].begin(), line[current_token].end(),aux.begin(), ::tolower);
                        solveDisplacement(line,current_token, aux+" eax, dword["+line[current_token+1]);
                        // this->output_file << aux << " eax, dword[" << line[current_token+1];
                        break;
                    case HashStringToInt("MULT"):
                        solveDisplacement(line,current_token,"imul dword[");
                        // this->output_file << "imul dword[" << line[current_token+1];
                        break;
                    case HashStringToInt("DIV"):
                        this->output_file << "cdq\n";
                        solveDisplacement(line,current_token,"idiv dword["+line[current_token+1]);
                        // this->output_file << "idiv dword[" << line[current_token+1];
                        break;
                    case HashStringToInt("JMP"):
                        solveDisplacement(line,current_token,"jmp "+line[current_token+1]);
                        // this->output_file << "jmp " <<line[current_token+1];
                        break;
                    case HashStringToInt("JMPP"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"jg " + line[current_token+1]);
		                // this->output_file << "jg " << line[current_token+1];
                        break;
                    case HashStringToInt("JMPN"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"jl "+ line[current_token+1]);
		                // this->output_file << "jl " << line[current_token+1];
                        break;
                    case HashStringToInt("JMPZ"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"je "+line[current_token+1]);
		                // this->output_file << "je " << line[current_token+1];
                        break;
                    
                    default:
                        break;
                }                
                break;
            case DATA:

                break;
            
            default:
                break;
        }

		// else if(this->master.substr(0, 3) == "JMP"){
		// 	// Deal with jumps
		// 	if(this->master.length() == 3){
		// 		// Unconditional jump
		// 		file << "jmp " << this->args[0] << endl;				
		// 	}
		// 	else {
		// 		// Compare eax with 0
		// 		file << "cmp eax, 0" << endl;
				
		// 		switch(this->master[3]){
		// 			case 'P':
		// 				file << "jg " << this->args[0] << endl;
		// 				break;
		// 			case 'N':
		// 				file << "jl " << this->args[0] << endl;
		// 				break;
		// 			case 'Z':
		// 				file << "je " << this->args[0] << endl;
		// 				break;
		// 			default:
		// 				break;			
		// 		};
		// 	};
		// }
		// else if(this->master == "COPY"){
		// 	// Copies first element into eax
		// 	file << "mov eax, dword[" << this->args[0];
			
		// 	// Check if a '+' was used in the invented assembler
		// 	if(this->args[1] == "+"){
		// 		file << " + " << this->args[2];
		// 	};
			
		// 	file << "]" << endl;
			
		// 	// Copies eax into second element
		// 	if(this->args[1] == "+"){
		// 		// Use 3rd argument
		// 		file << "mov dword[" << this->args[3];
			
		// 		// Check if a '+' was used in the invented assembler
		// 		if(this->args[4] == "+"){
		// 			file << " + " << this->args[5];
		// 		};
			
		// 		file << "], eax" << endl;				
		// 	} 
		// 	else {
		// 		file << "mov dword[" << this->args[1];
			
		// 		// Check if a '+' was used in the invented assembler
		// 		if(this->args[2] == "+"){
		// 			file << " + " << this->args[3];
		// 		};
			
		// 		file << "], eax" << endl;	
		// 	};
		// }
		// else if(this->master == "LOAD"){
		// 	file << "mov eax, dword[" << this->args[0];
			
		// 	// Check if a '+' was used in the invented assembler
		// 	if(this->args[1] == "+"){
		// 		file << " + " << this->args[2];
		// 	};
			
		// 	file << "]" << endl; 
		// }
		// else if(this->master == "STORE"){
		// 	file << "mov dword[" << this->args[0];
		
		// 	// Check if a '+' was used in the invented assembler
		// 	if(this->args[1] == "+"){
		// 		file << " + " << this->args[2];
		// 	};
		
		// 	file << "], eax" << endl;	
		// }
    }

    this->output_file.close();
}