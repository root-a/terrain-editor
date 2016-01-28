//------------------------------------------------------------------------------
//  consoletestapp.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "consoletestapp.h"

namespace Demos
{
using namespace IO;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
ConsoleTestApp::Run()
{
    // check command line args
    if (this->args.GetBoolFlag("-help"))
    {
        n_printf("Console Testing App.\n"
                 "-help    -- display this help\n");
        return;
    }

    String binPath = this->args.GetString("-folder", "bin:");
    
    
	Array<String> files = IoServer::Instance()->ListFiles(binPath, "*.exe");
    

    // for each file...
    IndexT i;
	for (i = 0; i < files.Size(); i++)
    {
		n_printf("%s\n", files[i].AsCharPtr());
    }   
}

} // namespace Demos