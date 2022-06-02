#pragma once
#include <cstdint>


namespace zombies
{
	uintptr_t aEntityList = 0x018E73C0;
	uintptr_t aViewMatrix = 0x008E870C;


	namespace Offsets {
		uintptr_t oLocation = 0x18;
		uintptr_t oHealth = 0x1C8;
		uintptr_t oDistanceBetweeen = 0x88;

		uintptr_t NoClip = 0x018EF2A4;
	}
}