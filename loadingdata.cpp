#include "loadingdata.h"

int DataHolder::getm_id() const{
    return this->m_id;
}
QString DataHolder::getLabelName() const{
    return this->getLabelName();
}

LoadingData::LoadingData()
{}

int LoadingData::increment_Index(){
    auto lock=std::lock_guard(LoadingData::index_mutex);
    int i=++LoadingData::data_index;
    return i;
}

void LoadingData::addDataHolder(QString &holderLabel){
    int id=increment_Index();
    UShDataHolder holder=std::make_unique<DataHolder>(holderLabel,id);
    this->holders.emplace_back(std::move(holder));
}

bool LoadingData::removeHolder(int id){
    for(auto iterator=this->holders.cbegin();iterator!=holders.cend();++iterator){
        //delete if holder matches id of id
        if(id==iterator.base()->get()->getm_id()){
            holders.erase(iterator);
            return true;
        }
    }
    return false;
}

QString LoadingData::getTopHolderString() const{
    return this->holders.cend().base()->get()->getLabelName();
}
