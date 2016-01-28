//------------------------------------------------------------------------------
//  demoproject.cc
//  (C) 2013-2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "netdemogameapplication.h"
#include "timing/calendartime.h"

ImplementNebulaApplication()

using namespace Demos;
using namespace Util;

//------------------------------------------------------------------------------
/**
*/
void
NebulaMain(const CommandLineArgs& args)
{
    DemoProjectApplication app;
    app.SetCompanyName("LTU - Lulea University of Technology");
    app.SetAppTitle("Demo Project");	
    app.SetCmdLineArgs(args);
	Util::String random;
	random.Format("TestNet%d", Timing::CalendarTime::GetSystemTime().GetMilliSecond());
	app.SetAppTitle(random);	
    if (app.Open())
    {
        app.Run();
        app.Close();
    }
    app.Exit();
}