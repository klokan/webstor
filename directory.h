#include <string>
#include <vector>

namespace webstor
{

class AbstractDirectoryReader {

public:
    ///@brief List all files in directory.
    ///@details list all files in <b>directory</b>, returned paths are relative
    virtual void listFiles( const std::string &directory, std::vector< std::string * > * result /* out */ ) = 0;

};

#ifdef __unix__
class PosixDirectoryReader : public AbstractDirectoryReader {

public:
    void listFiles( const std::string &directory, std::vector< std::string * > * result  );

};
#else
//FIXME: support for others platforms
#error "Only unix is supported now"
#endif

class DirectoryReader : public AbstractDirectoryReader {

#ifdef __unix__
private:
    PosixDirectoryReader impl;
#endif

public:
    DirectoryReader();

    void listFiles( const std::string &directory, std::vector< std::string * > * result  ) {
        this->impl.listFiles( directory, result );
    }
};

} // namespace webstor
