#ifndef GLOBALSTORE_H
#define GLOBALSTORE_H
#include "headers.h"

class Options;
class peFile;

//这个类存储了一些横跨全局的信息，比如设置、pe文件等，为了方便，所有变量还是使用public
class GlobalStore
{
public:
    static GlobalStore * getInstanceP();

    Options* options = Q_NULLPTR;
    peFile* pefile = Q_NULLPTR;
    virtual ~GlobalStore();
private:
    static GlobalStore* g;
    GlobalStore();
};

#endif // GLOBALSTORE_H
