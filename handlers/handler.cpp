#include <handlers.h>
#include <data.h>

Handlers::SharedFilesVector Handlers::getFilesInDir(const std::string &directory){
    //create a path mapping to the directory
    const Handlers::FilePath p(directory);
    //check if it exists, throw if doesn't
    std::string error_message=p.filename().string()+" doesn't appear to exist";
    assert (boost::filesystem::exists(p));

    Handlers::SharedFilesVector filesInDIr=std::make_shared<Handlers::FilesVector>();

    if(boost::filesystem::is_directory(p)){
        recursiveGetFiles(p,filesInDIr);
    }else{
        filesInDIr->push_back(p);
    }
    return filesInDIr;
}

Handlers::SharedFilesVector Handlers::getFilesOfMultimedia(const SharedFilesVector filesInDir){
    SharedFilesVector files=std::make_shared<Handlers::FilesVector>();
    std::vector<Handlers::VideoType> types=Handlers::VideoType::getAll();

    //check that all files in types match that type, if not the don't add them to the list to be returned
    for(Handlers::FilePath &path: *filesInDir){
        for(Handlers::VideoType &type:types){
            if(path.extension()==(std::string(".")+type.getType())){
                files->push_back(path);
            }
        }
    }

    return files;
}

Handlers::PlayerHandler::PlayerHandler(){
    //add mp4, 3gp, and ts as default types to search for
    videoTypes.push_back("mp4");
    videoTypes.push_back("3gp");
    videoTypes.push_back("ts");
}

void Handlers::PlayerHandler::updateDirs(){

}
