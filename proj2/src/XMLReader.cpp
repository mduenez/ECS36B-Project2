#include "XMLReader.h"
#include <expat.h>
#include <queue>

struct CXMLReader::SImplementation{
    std::shared_ptr<CDataSource> DSource;
    bool reachedEnd = false;
    char PeekChar() {
        if (DSource->End()) return '\0';
        char c;
        DSource->Peek(c);
        return c;
    }
    char GetChar() {
        char c;
        if (DSource->Get(c)) return c;
        reachedEnd = true;
        return '\0';
    }
};
/**
 * @brief Constructs an XML reader.
 * @param src Shared pointer to the data source to read XML from.
 */

CXMLReader::CXMLReader(std::shared_ptr< CDataSource > src){
    DImplementation = std::make_unique<SImplementation>();
    DImplementation->DSource = src;
    DImplementation->reachedEnd = false;
}
/**
 * @brief Destructor for the XML reader.
 */
CXMLReader::~CXMLReader(){

}
/**
 * @brief Checks if you reached the end of the XML input
 * @return True if end of XML stream reached, false otherwise.
 */
bool CXMLReader::End() const{
    return DImplementation->reachedEnd;
}
/**
 * @brief Reads the next XML entity.
 * @param entity Reference to store the entity data.
 * @param skipcdata Whether to skip CDATA content.
 * @return True if an entity was successfully read, false otherwise.
 */
bool CXMLReader::ReadEntity(SXMLEntity &entity, bool skipcdata){
  ;

}
