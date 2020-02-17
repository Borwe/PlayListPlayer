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
        void setType(std::string &type){
            this->type=type;
        }

        //getters
        long getPID()const override{
            return pid;
        }
        std::string getType()const{
            return type;
        }
        static std::vector<VideoType> getAll();

        static ShVideoType getVideoTypeByType(const std::string &type);

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

        static ShDate getDateByDate(const Date &date);

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

    class Video:DBMethods{
    private:
        long pid;
        std::string location;
        std::string name;
        long long seekTime;

        //for the DB access
        long videoTypeID;
        long dateID;

    public:
        Video(const std::string &location,const std::string &name,
              const long long seekTime);
        Video(const Video *video);
        Video();

        //For DB
        template<typename Action>
        void persist(Action &a);
        void save()override;
        void unSave()override;
        static long countItems();

        //setters
        void setPID(const long pid)override{
            this->pid=pid;
        }

        void setLocation(const std::string &location){
            this->location=location;
        }

        void setName(const std::string &name){
            this->name=name;
        }

        void setType(const VideoType &type){
            this->videoTypeID=type.getPID();
        }

        void setDate(const Date &date){
            this->dateID=date.getPID();
        }

        void setSeek(const long long &seek){
            this->seekTime=seek;
        }

        //getters
        long getPID()const override{
            return pid;
        }

        std::string getLocation()const{
            return this->location;
        }

        std::string getName()const{
            return this->name;
        }

        long getVideoTypeID()const{
            return this->videoTypeID;
        }

        long getDateID()const{
            return this->dateID;
        }

        long long getSeekTime()const{
            return this->seekTime;
        }
    };

    class PlayList:DBMethods{
    private:
        long pid;
        std::string name;
        std::string location;
        std::vector<long> videoIDs;
        long dateLastPlayed;

        //for holding values as json of videoIDS
        std::string vids_ids_json;
    public:
        PlayList();
        PlayList(const std::string &&name,
                 const std::string &&location,
                 const std::vector<long> &videoIDs,
                 const long &dateLastPlayed);
        PlayList(const PlayList *playlist);

        //DB
        template<typename Action>
        void persist(Action &a);
        void save();
        void unSave();
        static std::vector<PlayList> getAll();
        static long countItems();

        //setters
        void setPID(long id) override{
            this->pid=id;
        }

        //getters
        long getPID()const override{
            return pid;
        }
        std::string getName()const{
            return name;
        }
        std::string getLocation()const{
            return location;
        }
        std::vector<long> getVideoIDs()const;

        long getDateLastPlayed(){
            return dateLastPlayed;
        }
    };
}

#endif // DATA_H
