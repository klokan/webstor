#include <string>
#include <vector>

namespace webstor
{

class AbstractDirectoryReader {

public:
    virtual bool isDirectory ( const std::string &directory ) = 0;
    ///@brief List all files in directory.
    ///@details list all files in <b>directory</b>, returned paths are relative
    virtual void listFiles( const std::string &directory, std::vector< std::string * > * result /* out */ ) = 0;

};

class PosixDirectoryReader : public AbstractDirectoryReader {

public:
    bool isDirectory ( const std::string &directory );
    void listFiles( const std::string &directory, std::vector< std::string * > * result  );

};

class DirectoryReader : public AbstractDirectoryReader {

private:
    PosixDirectoryReader impl;

public:
    DirectoryReader(){};

    bool isDirectory ( const std::string &directory ) {
        return this->impl.isDirectory( directory );
    }

    void listFiles( const std::string &directory, std::vector< std::string * > * result  ) {
        this->impl.listFiles( directory, result );
    }
};

} // namespace webstor
