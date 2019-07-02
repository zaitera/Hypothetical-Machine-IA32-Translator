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
        this->output_file << metadata <<" + " << std::stoi(line[current_token+3])*4;
    };       
    if (!used)
    {
        this->output_file << metadata;
    }
    if(line[current_token].substr(0, 3) != "JMP" && line[current_token]!="S_INPUT" && line[current_token]!="S_OUTPUT")
        this->output_file << "]";
    this->output_file << "\n";
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
    std::vector<std::string> functs_to_load;
    for(size_t i = 0; i != this->file_being_translated.size(); i++ )
    {
        auto line = std::get<1>(this->file_being_translated[i]);
        current_token = 0;
        this->output_file << ";;;;";
        for (size_t i = 0; i < line.size(); i++)
        {
            this->output_file <<line[i]<<" ";
        }
		this->output_file << ";;;;\n";

        if (line[0] == "SECTION")
        {
            if(line[1] == "TEXT")
            {
                section = TEXT;
                this->output_file<<"section .text\n";
                this->output_file<<"_start:\n";                
            }else if(line[1] == "DATA") 
            {
                section = DATA;
                this->output_file<<"section .data\n";
				this->output_file << "__min_int db \"-2147483648\"" << std::endl;
            }
            continue;
        }
        if(line.size()>=2 && line[1] == ":")
        {
            this->output_file<<line[0]+": ";
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
                        break;
                    case HashStringToInt("MULT"):
                        solveDisplacement(line,current_token,"imul dword["+line[current_token+1]);
                        break;
                    case HashStringToInt("DIV"):
                        this->output_file << "cdq\n";
                        solveDisplacement(line,current_token,"idiv dword["+line[current_token+1]);
                        break;
                    case HashStringToInt("JMP"):
                        solveDisplacement(line,current_token,"jmp "+line[current_token+1]);
                        break;
                    case HashStringToInt("JMPP"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"jg " + line[current_token+1]);
                        break;
                    case HashStringToInt("JMPN"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"jl "+ line[current_token+1]);
                        break;
                    case HashStringToInt("JMPZ"):
                        this->output_file << "cmp eax, 0" << std::endl;
                        solveDisplacement(line,current_token,"je "+line[current_token+1]);
                        break;
                    case HashStringToInt("COPY"):
                        solveDisplacement(line,current_token,"mov ebx, dword["+line[current_token+1]);
                        if (line.size()-current_token>5 && line[line.size()-2] == "+")
                        {
                            this->output_file << "mov dword["<< line[line.size()-3]<<" + " << std::stoi(line[line.size()-1])*4<<"], ebx\n";                        
                        }else
                        {
                            this->output_file << "mov dword["<< line[line.size()-1]<<"]"<<", ebx\n";
                        }
                        break;
                    case HashStringToInt("LOAD"):
                        solveDisplacement(line,current_token,"mov eax, dword["+line[current_token+1]);
                        break;
                    case HashStringToInt("STORE"): 
                        if(line.size()-current_token>3 && line[current_token+2] == "+"){
                            this->output_file << "mov dword["<<line[current_token+1] <<" + " << std::stoi(line[current_token+3])*4<<"], eax\n";
                        }else
                        {
                            this->output_file << "mov dword["<<line[current_token+1] <<"], eax\n";
                            
                        }
                        break;
                    case HashStringToInt("INPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";
                        
                        solveDisplacement(line,current_token,"push dword["+line[current_token+1]);
                        
                        this->output_file << "call LerInteiro\n";

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("readInt");
                        break;
                    case HashStringToInt("OUTPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";
                        
                        solveDisplacement(line,current_token,"push dword["+line[current_token+1]);
                        
                        this->output_file << "call EscreverInteiro\n";

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("writeInt");
                        break;
                    case HashStringToInt("C_INPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";
                        
                        solveDisplacement(line,current_token,"push dword["+line[current_token+1]);
                        
                        this->output_file << "call LerChar\n";

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("readChar");
                        break;
                    case HashStringToInt("C_OUTPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";
                        
                        solveDisplacement(line,current_token,"push dword["+line[current_token+1]);
                        
                        this->output_file << "call EscreverChar\n";

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("writeChar");
                        break;
                    case HashStringToInt("S_INPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";
                        
                        this->output_file << "push " << line[line.size()-1] << "\n";
                    
                        solveDisplacement(line,current_token,"mov esi, "+line[current_token+1]);
                        this->output_file << "push esi\n";

                        this->output_file << "call LerString\n";	

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("readString");
                        break;
                    case HashStringToInt("S_OUTPUT"):
                        this->output_file << "push ebx\n";
                        this->output_file << "push ecx\n";
                        this->output_file << "push edx\n";

                        this->output_file << "push " << line[line.size()-1] << "\n";
                        

                        solveDisplacement(line,current_token,"mov esi, "+line[current_token+1]);
                        this->output_file << "push esi\n";

                        
                        this->output_file << "call EscreverString\n";	

                        this->output_file << "pop edx\n";
                        this->output_file << "pop ecx\n";
                        this->output_file << "pop ebx\n";
                        functs_to_load.push_back("writeString");
                        break;
                    case HashStringToInt("STOP"):{
                        this->output_file << "__exit:\n";
                        this->output_file << "mov eax, 1\n";
                        this->output_file <<"mov ebx, 0\n";
                        this->output_file <<"int 0x80\n";
                        this->output_file <<"\n";
                        this->output_file <<";USED IO FUNCTIONS:\n";
                        this->output_file <<"\n";
                        
                        //delete duplicates
                        sort( functs_to_load.begin(), functs_to_load.end() );
                        functs_to_load.erase( unique( functs_to_load.begin(), functs_to_load.end() ), functs_to_load.end());
                        
                        std::string text_line;
                        std::fstream funct_code;
                        for (size_t i = 0; i < functs_to_load.size(); i++)
                        {
                            funct_code.open("./functs/"+functs_to_load[i]+".s",std::ios::in);
                            while (std::getline(funct_code, text_line))
                            {
                                this->output_file<<text_line<<"\n";
                                
                            }
                            this->output_file<<"\n";
                            funct_code.close();
                        }
                    }
                    default:
                        break;
                }                
                break;
            case DATA:
                if (line[current_token] == "CONST")
                {
                    long pos = this->output_file.tellp(); // gets the current position of the buffer 
                    this->output_file.seekp(pos - 2); //to remove the : char which is not used in ia32
                    this->output_file <<" dd " << line[current_token+1] << "\n";
                }else if (line[current_token] == "SPACE")
                {
                    size_t spaces = 0;
                    if (line.size()-current_token>1)
                    {
                        spaces = std::stoi(line[current_token+1]);
                    }else
                    {
                        spaces = 1;
                    }
                    long pos = this->output_file.tellp(); // gets the current position of the buffer 
                    this->output_file.seekp(pos - 2); //to remove the : char which is not used in ia32
                    this->output_file <<" dd ";
                    
                    do{
                        this->output_file <<"00,";
                        spaces--;
                    }while(spaces > 0);
                    pos = this->output_file.tellp(); // gets the current position of the buffer 
                    this->output_file.seekp(pos - 1); //to remove the last ,  added in the loop
                    this->output_file <<"\n";      
                }
                
                
                break;
            
            default:
                break;
        }
        

	
        // STOP
        // 
    }

    this->output_file.close();
}