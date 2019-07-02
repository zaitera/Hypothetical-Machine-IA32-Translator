#include <linker.h>

typedef unsigned char BYTE;

Linker::Linker(std::string name)
{
    this->file_name = name;
}

Linker::~Linker(){}

std::vector<BYTE> readBinaryReturnBuffer(const char* filename)
{
    // open the file:
    std::streampos fileSize;
    std::ifstream file(filename, std::ios::binary);

    // get its size:
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data:
    std::vector<BYTE> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    return fileData;
}


void Linker::link(void)
{
    std::string command("nasm -f elf -o "+ this->file_name+".o "+this->file_name+".s");
    system(command.c_str());
    command = "objcopy -O binary --only-section=.text "+this->file_name+".o"+" text_sec.bin";
    system(command.c_str());
    command = "objcopy -O binary --only-section=.data "+this->file_name+".o"+" data_sec.bin";
    system(command.c_str());
    
    elfio writer;
    writer.create( ELFCLASS32, ELFDATA2LSB );
    writer.set_os_abi( ELFOSABI_LINUX );
    writer.set_type( ET_EXEC );
    writer.set_machine( EM_386 );


    section* text_sec = writer.sections.add( ".text" );
    text_sec->set_type( SHT_PROGBITS );
    text_sec->set_flags( SHF_ALLOC | SHF_EXECINSTR );
    text_sec->set_addr_align( 0x10 );

    std::vector<BYTE> text_vec = readBinaryReturnBuffer("text_sec.bin");
    char text[text_vec.size()];

    for (size_t i = 0; i < text_vec.size(); i++)
    {
        text[i] = text_vec[i];
    }
    
    std::cout<<text<<" - "<<sizeof(text)<<std::endl;



    text_sec->set_data( text, sizeof(text) );
    segment* text_seg = writer.segments.add();
    text_seg->set_type( PT_LOAD );
    text_seg->set_virtual_address( 0x08048000 );
    text_seg->set_physical_address( 0x08048000 );
    text_seg->set_flags( PF_X | PF_R );
    text_seg->set_align( 0x1000 );
    text_seg->add_section_index( text_sec->get_index(),
    text_sec->get_addr_align() );

    section* data_sec = writer.sections.add( ".data" );
    data_sec->set_type( SHT_PROGBITS );
    data_sec->set_flags( SHF_ALLOC | SHF_WRITE );
    data_sec->set_addr_align( 0x4 );

    std::vector<BYTE> data_vec = readBinaryReturnBuffer("data_sec.bin");
    char data[data_vec.size()];
    for (size_t i = 0; i < data_vec.size(); i++)
    {
        data[i] = data_vec[i];
    }
    

    std::cout<<data<<" - "<<sizeof(data)<<std::endl;


    data_sec->set_data( data, sizeof(data) );
    segment* data_seg = writer.segments.add();
    data_seg->set_type( PT_LOAD );
    data_seg->set_virtual_address( 0x0804a000 );
    data_seg->set_physical_address( 0x0804a000 );
    data_seg->set_flags( PF_W | PF_R );
    data_seg->set_align( 0x10 );
    data_seg->add_section_index( data_sec->get_index(),
    data_sec->get_addr_align() );
    writer.set_entry( 0x08049000 );

    writer.save( this->file_name.c_str());
    command = "chmod +x " + this->file_name;
    system(command.c_str());
    command = "rm "+this->file_name+".o";
    system("rm data_sec.bin");
    system("rm text_sec.bin");
    system(command.c_str());
}