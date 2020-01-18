#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <memory>




namespace Handlers {
    class Date;
    class VideoType;
    //for returning a shared versions
    using ShDate=std::shared_ptr<Date>;
    using ShVideoType=std::shared_ptr<VideoType>;

    class DBMethods{
    public:
        virtual void setPID(long id){}
        virtual long getPID() const{}
        virtual void save(){}
        virtual void unSave(){}
    };

    class VideoType:DBMethods{
    private:
        long pid;
        std::string type;
    public:
        VideoType(const std::string &&type);
        VideoType(const std::string &type);
        VideoType();

        //for DB
        template<class Action>
        void persist(Action &a);


        void save()override;
        void unSave() override;

        //setters
        void setPID(long pid)override{
            this->pid=pid;
        }

        //getters
        long getPID()const override{
            return pid;
        }
        std::string getType()const{
            return type;
        }

        bool operator==(VideoType &v1){
            if(v1.getPID()==getPID()){
                if(v1.getType()==getType()){
                    return true;
                }
            }
            return false;
        }
    };

    class Date:DBMethods {
    private:
        int day;
        int month;
        int year;
        long pid;



    public:
        Date(int day,int month,int year);
        Date(const Date *date);
        Date();

        template<typename Action>
        void persist(Action &a);

        /**
         * @brief operator ==
         * @param d2
         * @return
         * used for comparing
         */
        bool operator==(Date &d2);

        /**
         * @brief save
         * @return
         */
        void save()override;
        /**
         * @brief unSave
         * Used for removing saved item from the database
         */
        void unSave()override;




        //getters
        long getPID() const override{
            return pid;
        }
        int getDay()const{
            return this->day;
        }
        int getMonth()const{
            return this->month;
        }
        int getYear()const{
            return this->year;
        }

        //setters
        void setPID(long id) override;

        void setDay(int day){
            this->day=day;
        }
        void setMonth(int month){
            this->month=month;
        }
        void setYear(int year){
            this->year=year;
        }
    };

}

#endif // DATA_H
