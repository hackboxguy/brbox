#include "MPlayer.h"
using namespace std;
/*****************************************************************************/
MPlayer::MPlayer(std::string devnode)//:I2CBusAccess(devnode)
{
	edid_dev_node=devnode;
	graphics_out_sts=MPLAYSRV_GRAPHICS_OUT_ENABLE;//by default graphics out is enabled
}
/*****************************************************************************/
MPlayer::~MPlayer()
{
}
/*****************************************************************************/

