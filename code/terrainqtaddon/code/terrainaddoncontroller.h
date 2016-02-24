#pragma once
//------------------------------------------------------------------------------
/**
    @class QtTerrainAddon::TerrainAddonController
    
    
    
    (C) 2016 Mariusz Waclawek
*/
//------------------------------------------------------------------------------
#include <QDialog>

namespace Ui
{
	class TerrainAddonWindow;
}
namespace QtTerrainAddon
{
class TerrainAddonController : public QDialog
{
	Q_OBJECT
public:
	/// constructor
	TerrainAddonController();
	/// destructor
	virtual ~TerrainAddonController();

	

protected:
	/// qts close handler
	virtual void closeEvent(QCloseEvent * event);
private slots:
	

	/// parameters modified
	bool IsModified();
	/// set modified
	void SetModified();


private:

	Ui::TerrainAddonWindow* ui;
}; 

} // namespace QtTerrainAddon
//------------------------------------------------------------------------------