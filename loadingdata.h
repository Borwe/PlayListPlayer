#ifndef LOADINGDATA_H
#define LOADINGDATA_H

#include <QString>
#include <mutex>
#include <vector>
#include <memory>

class DataHolder{
private:
    //For holding the label name
    QString m_label;
    //for holding the id of the label
    int m_id;

public:
    //main constructor
    DataHolder(QString label,int id):m_label(label),m_id(id){}
    //should not have any copy constructor
    DataHolder(DataHolder &holder)=delete ;
    DataHolder &operator=(DataHolder &holder)=delete ;
    //can be movable
    DataHolder(DataHolder &&holder)=default;
    DataHolder &operator=(DataHolder &&holder)=default;

    //getters
    QString getLabelName() const;
    int getm_id() const;
};

using UShDataHolder=std::unique_ptr<DataHolder>;

/**
 * @brief The LoadingData class
 * Used for storing information on the loading
 * bar data, to make it easier and automated
 */
class LoadingData
{
    inline static std::mutex index_mutex;
    inline static int data_index=0;
    static int increment_Index();
    std::vector<UShDataHolder> holders;
public:
    void addDataHolder(QString &holderLabel);
    /**
     * @brief removeHolder
     * @param id
     * @return
     * Used for removing DataHolder object, once it's loading has ended
     */
    bool removeHolder(int id);

    /**
     * @brief getTopHolderString
     * @return a Qstring that displays information of top most holder string
     */
    QString getTopHolderString()const;
    LoadingData();
};

#endif // LOADINGDATA_H
