#pragma once

struct Tile {
	int image;
	bool air = false;
	bool coll;
	bool isLadder = false;
	bool destroyZone = false;
};
