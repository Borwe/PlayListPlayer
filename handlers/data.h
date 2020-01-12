#ifndef DATA_H
#define DATA_H

#include <string>
#include <vector>
#include <memory>




namespace Handlers {
    class Date;
    //for returning a shared version of Date
    using ShDate=std::shared_ptr<Date>;

    class DBMethods{
    public:
        virtual void setPID(long id){}
        virtual long getPID() const{}
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
        void save();
        /**
         * @brief unSave
         * Used for removing saved item from the database
         */
        void unSave();




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
