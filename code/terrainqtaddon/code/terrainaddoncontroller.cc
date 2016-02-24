//------------------------------------------------------------------------------
//  terrainaddoncontroller.cc
//  (C) 2016 Mariusz Waclawek
//------------------------------------------------------------------------------
#include "stdneb.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QPixmap>
#include "terrainaddoncontroller.h"
#include "ui_terrainaddonwindow.h"
#include "math/float4.h"
#include "io/ioserver.h"
#include "graphics/graphicsinterface.h"
#include <QMessageBox>

using namespace IO;
using namespace Util;
using namespace Math;
using namespace Graphics;
namespace QtTerrainAddon
{

//------------------------------------------------------------------------------
/**
*/
TerrainAddonController::TerrainAddonController() 
{
	// create ui
	this->ui = new Ui::TerrainAddonWindow;

	// setup ui
	this->ui->setupUi(this);
	this->setWindowFlags(Qt::Tool);

}

//------------------------------------------------------------------------------
/**
*/
TerrainAddonController::~TerrainAddonController()
{
	
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainAddonController::closeEvent(QCloseEvent * event)
{
	if (this->IsModified())
	{
		QMessageBox box;
		box.setText("The preset has been modified");
		box.setInformativeText("Do you want to save your changes");
		box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
		box.setDefaultButton(QMessageBox::Save);
		int ret = box.exec();
	}
	QDialog::closeEvent(event);
}

bool TerrainAddonController::IsModified()
{
	return true;
}

void TerrainAddonController::SetModified()
{

}

} // namespace QtTerrainAddon