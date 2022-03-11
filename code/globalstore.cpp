#include "globalstore.h"
#include "options.h"
#include "pefile.h"

GlobalStore* GlobalStore::g = Q_NULLPTR;

GlobalStore::GlobalStore()
{

}
GlobalStore::~GlobalStore(){
    delete options;
    delete pefile;
    delete g;
}

GlobalStore * GlobalStore::getInstanceP(){
    if(g == Q_NULLPTR){
        g = new GlobalStore();
        g->pefile = new peFile();
        g->options = new Options();
    }
    return g;
}
