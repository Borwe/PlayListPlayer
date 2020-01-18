#include <data.h>
#include <Poco/DateTime.h>
#include <iostream>
#include <algorithm>

//for db
#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Sqlite3.h>
#include <mutex>
#include <shared_mutex>
#include <memory>
#include <condition_variable>
#include <stdexcept>
#include <functional>

namespace DBAccess {
    using Session=Wt::Dbo::Session;
    using ShSession=std::shared_ptr<Session>;

    namespace  {
        ShSession session=nullptr;
    }

    std::mutex m_session;
    std::once_flag flag;

    //structs for handling pid's of objects
    struct DatePID{
        static std::shared_mutex m_lock;
        static long p_id;
    };
    std::shared_mutex DatePID::m_lock;
    long DatePID::p_id=0;
    struct VideoTypePID{
        static std::shared_mutex m_lock;
        static long p_id;
    };
    std::shared_mutex VideoTypePID::m_lock;
    long VideoTypePID::p_id=0;

    //function to handle incremeneting PID's
    template<typename PID>
    void incrementPID(){
        std::unique_lock<std::shared_mutex> ul(PID::m_lock);
        ++PID::p_id;
        ul.unlock();
    }

    //function to get PID values of passed in class
    template<typename PID>
    long getPID(){
        std::shared_lock<std::shared_mutex> sl(PID::m_lock);
        return PID::p_id;
    }

    //function to set the PID
    template<typename PID>
    void setPID(long pid){
        std::unique_lock<std::shared_mutex> ul(PID::m_lock);
        PID::p_id=pid;
    }
    /**
     *Pass a Type of Class, of which is PID type is a clas of type [Class]PID
     */
    template<typename Class,typename PID>
    void getMaxPID(){
        Wt::Dbo::Transaction t(*session);
        Wt::Dbo::collection<Wt::Dbo::ptr<Class>> collection=session->find<Class>().orderBy("pid").resultList();
        //move to last and get it's id
        if(collection.empty()){
            setPID<PID>(0);
        }else{
            auto fin=collection.begin();
            unsigned int p=0;
            while(p<collection.size()-1 ){
                ++fin;
                ++p;
            }
            setPID<PID>(fin->get()->getPID());
        }
    }


    ShSession getSession(){
        std::lock_guard<std::mutex> l(m_session);
        //setup the session and only do it once
        std::call_once(flag,[](ShSession &session){
            std::unique_ptr<Wt::Dbo::backend::Sqlite3> sqlite3(new Wt::Dbo::backend::Sqlite3("data.db"));
            session=std::make_shared<Session>();
            session->setConnection(std::move(sqlite3));



            //try map to Date table
            try {
                session->mapClass<Handlers::Date>("date_table");
                session->mapClass<Handlers::VideoType>("video_types");

            } catch (Wt::Dbo::Exception &ex) {
    //            std::cerr<<"\nERROR: "<<ex.what();
            }


            //try create tables here now
            try {
                session->createTables();
            } catch (Wt::Dbo::Exception &ex) {
    //            std::cerr<<"\nERROR: "<<ex.what();
            }


            //get the max pid of objects mapped
            getMaxPID<Handlers::Date,DatePID>();
            getMaxPID<Handlers::VideoType,VideoTypePID>();


        },session);

        return session;
    }



    /**
     * Pass a class with it's [Class]PID struct to use, and let it do
     * the magic of adding object to db and updating objs.pid to match the updated
     */
    template<typename Class,typename PID>
    void startSaving(Class &obj,std::unique_ptr<Class> &&uObj){
            DBAccess::ShSession session=getSession();
            Wt::Dbo::Transaction trans(*session);


            DBAccess::incrementPID<PID>();
            uObj->setPID(DBAccess::getPID<DBAccess::DatePID>());
            auto ptr2=session->add(std::move(uObj));
            obj.setPID(ptr2->getPID());
            ptr2.purge();
    }

    /**
     * Pass obj you want to update from database, with a function that
     * shows how to update the pointer to the object in the database
     */
    template<typename Class>
    void startUpdating(Class &obj,std::function<void(Wt::Dbo::ptr<Class> &,Class &)> &func ){
        //meaning has already been saved before so, now just update it on the database
        DBAccess::ShSession session=DBAccess::getSession();
        Wt::Dbo::Transaction trans(*session);
        //get item matching pid of current object
        Wt::Dbo::ptr<Class> ptr=session->find<Class>().where("pid=?").bind(obj.getPID()).resultValue();
        //check that it exists
        if(ptr.get()!=nullptr){
            //execute function
            func(ptr,obj);
        }else{
            throw std::runtime_error("Please make sure you reset PID to -1, can't update an object which was not saved in database before");
        }
    }

    /**
     *Pass in an object to be used to delete of Type Class from the database
     */
    template<typename Class>
    void startDelete(Class &obj){

        if(obj.getPID()>=0){
            DBAccess::ShSession session=DBAccess::getSession();
            //start a transaction
            Wt::Dbo::Transaction trans(*session);

            //now try delete object
            Wt::Dbo::ptr<Class> result=session->find<Class>().where("pid=?").bind(obj.getPID()).resultValue();
            if(result.get()!=nullptr){
                //now delete this obj
                result.remove();
                //and reset current pid to -1
                obj.setPID(-1);
            }
        }else{
            std::cout<<"No point deleting something that doesn't even exist";
        }
    }
}


Handlers::Date::Date( int day,int month,int year):pid(-1){
    if(month>12 || month<0){
        throw std::runtime_error("month not valid");
    }
    if(day<0){
        throw std::runtime_error("can't have negative day");
    }
    if(year<0){
        throw std::runtime_error("can't have negative year");
    }
    //check that day actually exists in the given month
    int daysOfMonth=Poco::DateTime::daysOfMonth(year,month);
    if(day>daysOfMonth){
        throw std::runtime_error("the day passed exceeds number of days in month");
    }

    this->day=day;
    this->month=month;
    this->year=year;
}

Handlers::Date::Date():pid(-1){
    Poco::DateTime nowP;
    this->day=nowP.day();
    this->year=nowP.year();
    this->month=nowP.month();
}

Handlers::Date::Date(const Date *d):pid(-1){
    this->day=d->day;
    this->year=d->year;
    this->month=d->month;
    this->pid=d->pid;
}

void Handlers::Date::setPID(long pid){
    this->pid=pid;
}

bool Handlers::Date::operator==(Date &d){
    return (day==d.day && month==d.month && year==d.year)?true:false;
}

template<typename Action>
void Handlers::Date::persist(Action &a){
    Wt::Dbo::field(a,this->day,"day");
    Wt::Dbo::field(a,this->month,"month");
    Wt::Dbo::field(a,this->year,"year");
    Wt::Dbo::field(a,this->pid,"pid");
}

void Handlers::Date::save(){
    if(this->pid<0){
        //meaning not saved before, so we need to save it
        try {
            DBAccess::startSaving<Handlers::Date,DBAccess::DatePID>(*this,std::make_unique<Handlers::Date>(this->day,this->month,this->year));
        } catch (std::exception &ex) {
            std::cerr<<"ERROR: "<<ex.what()<<"\n";
        }
    }else{
        //meaning has already been saved before so, now just update it on the database
        std::function func=[](Wt::Dbo::ptr<Handlers::Date> &ptr,Handlers::Date &date){
                ptr.modify()->setDay(date.getDay());
                ptr.modify()->setMonth(date.getMonth());
                ptr.modify()->setYear(date.getYear());
        };
        DBAccess::startUpdating<Handlers::Date>(*this,func);
    }
}

void Handlers::Date::unSave(){
    DBAccess::startDelete(*this);
}

Handlers::VideoType::VideoType():pid(-1){

}

Handlers::VideoType::VideoType(const std::string &&type):pid(-1){
    this->type=type;
}

Handlers::VideoType::VideoType(const std::string &type):pid(-1){
    this->type=type;
}

template<typename Action>
void Handlers::VideoType::persist(Action &a){
    Wt::Dbo::field(a,pid,"pid");
    Wt::Dbo::field(a,this->type,"type");
}

void Handlers::VideoType::save(){

}

void Handlers::VideoType::unSave(){

}
