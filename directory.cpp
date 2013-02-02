#include "directory.h"
#include <sys/types.h>
#include <dirent.h>
#include <iostream>

namespace webstor {

#ifdef __unix__
void PosixDirectoryReader::listFiles( const std::string &directory, std::vector< std::string * > * result ) {
    DIR *dir = opendir (directory.c_str());
    struct dirent *directory_entry;
    if (dir != NULL)
    {
        while ( directory_entry = readdir( dir )  )
        {
            if ( directory_entry->d_type == DT_REG ) {
                result->push_back( new std::string( directory_entry->d_name ) );
            }
        }
    } else {
        throw std::string("directory '" + directory + "' can't be opened.");
    }
}
#endif

}

#ifdef TEST

int
main( int argc, char **argv )
{
    if ( argc != 2 )
    {
        std::cerr << "usage: directory directory_to_list" << std::endl;
        return 1;
    }
    try {
        webstor::PosixDirectoryReader directoryReader;
        std::vector< std::string * > *files = new std::vector< std::string * >();
        directoryReader.listFiles( std::string( argv[1] ) , files );
        for (std::vector< std::string * >::iterator iter = files->begin(); iter != files->end(); iter++) {
            std::cout << **iter << std::endl;
        }
    } catch (std::string & ex) {
        std::cerr << ex << std::endl;
        return 2;
    }
    return 0;
}

#endif
