#ifndef HANDLERS_H
#define HANDLERS_H

#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <map>
#include <memory>
#include <cassert>
#include <algorithm>

namespace Handlers
{
    using FilePath=boost::filesystem::path;
    using FilesVector=std::vector<boost::filesystem::path>;
    using SharedFilesVector=std::shared_ptr<FilesVector>;
    namespace  {

        void recursiveGetFiles(const FilePath pf,SharedFilesVector fileHolder){

            try {
                auto di=boost::filesystem::directory_iterator(pf);

                for(boost::filesystem::directory_entry &dir:di){
                    if(boost::filesystem::is_directory(dir.path()) && dir.path().string()!="./.." && dir.path().string()!="./."){
                            recursiveGetFiles(dir.path(),fileHolder);
                    }
                    if(boost::filesystem::is_regular_file(dir.path())){
                        fileHolder->push_back(dir.path());
                    }
                }
            } catch (std::exception &ex) {
                std::cerr<<"ERROR: "<<ex.what()<<"\n";
            }
        }
    }

    SharedFilesVector getFilesInDir(const std::string &directory="./");

    /**
     * @brief The PlayerHandler class
     * handles the creation and management of playlists.
     * - Playlists can be added based on directory that contains videos.
     * - TYpes to be considered as videos can also be added to the Player Handler
     * - Once a previous directory siezes to exist it should be removed from the Player Handler
     * - Player Handler stores all playlist directories added that contain atleast one video
     * - If in the process of adding a playlist, no video type is detected, consider adding a video type,
     *  to PlayerHandler else, the directory will be ignored and not considered as a directory
     *  containing any playlist.
     * - All the data is stored in data.db file where the executable is located
     */
    class PlayerHandler{
    private:
        std::vector<std::string> videoTypes;

        /**
         * @brief files
         * Used for storing files in a path inside a map.
         */
        std::map<FilePath,FilesVector> files;
        void updateDirs();

    public:
        PlayerHandler();

        /**
         * @brief getSupportedVideoTypes
         * @return
         */
        const std::vector<std::string> getSupportedVideoTypes()const{
            return videoTypes;
        }

        /**
         * @brief addVideoType
         * @param type
         */
        void addVideoType(const std::string &type){
            videoTypes.push_back(type);
        }
    };
}

#endif // HANDLERS_H
