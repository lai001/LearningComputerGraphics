#include "Keyboard.hpp"

namespace Keyboard
{
	const ModeFlag ModeFlag::Control = ModeFlag(0x0001);
	const ModeFlag ModeFlag::Alt = ModeFlag(0x0002);
	const ModeFlag ModeFlag::Shift = ModeFlag(0x0004);
	const ModeFlag ModeFlag::Super = ModeFlag(0x0008);
	const ModeFlag ModeFlag::CapsLock = ModeFlag(0x0010);
	const ModeFlag ModeFlag::NumLock = ModeFlag(0x0020);
}