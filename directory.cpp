#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <stdio.h>
#include <stack>
#include <string.h>

#include "directory.h"

namespace webstor {

#ifdef __unix__
void PosixDirectoryReader::listFiles( const std::string &dir, std::vector< std::string * > * result ) {
    std::stack< std::string > dir_stack;
    dir_stack.push( dir );
    while ( !dir_stack.empty() ) {
        std::string directory = dir_stack.top();
        dir_stack.pop();
        DIR *dir = opendir (directory.c_str());
        struct dirent *directory_entry;
        if (dir != NULL)
        {
            while ( directory_entry = readdir( dir )  )
            {
                bool rel = (strcmp ( directory.c_str() , "." ) == 0);
                if ( directory_entry->d_type == DT_REG ) {
                     if (rel) {
                         result->push_back( new std::string( directory_entry->d_name ) );
                     } else {
                         result->push_back( new std::string( directory + "/" + directory_entry->d_name ) );
                     }
                } else if ( directory_entry->d_type == DT_DIR ) {
                    if ( strcmp (directory_entry->d_name, ".") != 0 && strcmp (directory_entry->d_name, "..") != 0 ) {
                        if ( rel ) {
                            std::string new_dir( directory_entry->d_name );
                            dir_stack.push( new_dir );
                        } else {
                            std::string new_dir( directory + "/" + directory_entry->d_name );
                            dir_stack.push( new_dir );
                        }
                    }
                }
            }
        } else {
            throw std::string("directory '" + directory + "' can't be opened.");
        }
        closedir(dir);
    }
}

bool PosixDirectoryReader::isDirectory( const std::string &directory ) {
    struct stat buf;
    if ( stat( directory.c_str(), &buf) != -1 ) {
        return S_ISDIR( buf.st_mode );
    }
    return false;
}

}

#endif

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
        if ( directoryReader.isDirectory( std::string( argv[1] ) ) ) {
            std::vector< std::string * > *files = new std::vector< std::string * >();
            directoryReader.listFiles( std::string( argv[1] ) , files );
            for (std::vector< std::string * >::iterator iter = files->begin(); iter != files->end(); iter++) {
                std::cout << **iter << std::endl;
            }
        } else {
            std::cerr << argv[1] << " is not a directory." << std::endl;
        }
    } catch (std::string & ex) {
        std::cerr << ex << std::endl;
        return 2;
    }
    return 0;
}

#endif
