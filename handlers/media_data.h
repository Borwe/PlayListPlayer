#ifndef MEDIA_DATA_H
#define MEDIA_DATA_H

#include <string>
#include <data.h>
#include <vector>

namespace Handlers {
    class Video;

    /**
     * @brief The Playlist class
     * contains information on:
     * - name of playlist
     * - description of playlist
     * - local_location of wher it is stored
     * - boolean of played, if playlist started or not
     * - last time played if ever played.
     * - index of videos in playlist if played
     * - list ov videos in playlist
     */
    class Playlist{
    private:
        /**
         * @brief name
         * Hold name of the playlist
         */
        std::string name;

        /**
         * @brief description
         * Hold description of the playlist
         */
        std::string description;

        /**
         * @brief local_location
         * Hold information on the location/folder where
         * playlist is located
         */
        std::string local_location;

        /**
         * @brief played
         * mark if ever played or not
         */
        bool played;

        /**
         * @brief time
         * Mark the time last played
         */
        Date time_last_played;

        /**
         * @brief last_played_position
         * Mark last video in index this playlist played
         */
        long last_played_position;

        /**
         * @brief videos
         * Hold list of videos that this playlist has
         */
        std::vector<Video> videos;
    };


    /**
     * @brief The Video class
     * Hold information on the video file
     * - location
     * - name
     * - played or not
     * - last watch position if played
     */
    class Video{

    };
}

#endif // MEDIA_DATA_H
