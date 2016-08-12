#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <string>

using namespace std;
#include "ProgramOptions.hpp"
#include "FilePathUtils.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc,char **argv)
{
    string famx;
    string prefx;
    string vers;
    string dir;
    putils::ProgramOptions options;
    
    options.addOption("prefix","installation directory","/usr/local");
    options.addOption("family","fammily designation","none");
    options.addOption("version","version number","1.0.0");
    options.addOption("mod_name","name of module file","tmp.lua");
    options.parseCommandLine(argc,argv);

    prefx= options.getValue("prefix");
    
    if (!putils::isDirectory(prefx)) {
        cerr << "No such directory as " << prefx << "\n";
        cerr << "exiting now!\n";
        return EXIT_FAILURE;
    }

    vers = options.getValue("version");
    dir = prefx;
    
    dir += vers;
    dir += ".lua";

    string modulefilename = options.getValue("mod_name");
    
    std::ofstream out(modulefilename);
    if (options.wasSet("family")) {
        famx = options.getValue("family");
        out << "family(" << famx << ")\n"; 
    }    

    out << "local tpath = " << prefx << "\n"; 
    out << "local binpath = " << "pathJoin(tpath,\"/bin\")" << "\n";
    out << "local libpath = " << "pathJoin(tpath,\"/lib\")" << "\n";  
    out << "local incpath = " << "pathJoin(tpath,\"/include\")" << "\n";
    out << "prepend_path(CPATH,incpath) \n";
    out << "prepend_path(LIBRARY_PATH,libpath) \n";
    out << "prepend_path(PATH,binpath) \n";
    out << "prepend_path(LD_LIBRARY_PATH,libpath)\n";
    out << "prepend_path(INC_NCAR,\"-I \" .. incpath .. \" \")\n";
    out << "local lflags = \"-L\" .. libpath\n";
    out << "prepend_path(LIB_NCAR,\"-Wl,-rpath \" .. libpath .. lflags)"  << "\n";
        
    out.close();    
    return EXIT_SUCCESS;
}     
