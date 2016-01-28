#pragma once
//------------------------------------------------------------------------------
/**	
    @class Demos::ConsoleTestApp
    
    Empty console test app.
    
    (C) 2016 Individual contributors, see AUTHORS file
*/
#include "app/consoleapplication.h"

//------------------------------------------------------------------------------
namespace Demos
{
	class ConsoleTestApp : public App::ConsoleApplication
{
public:
    /// run the application
    virtual void Run();
    
private:
   
};

} // namespace Demos
//------------------------------------------------------------------------------
    