// Map.h
//ifndef

class Map {

	public:
		Map();

	private:
		vector<CellGroup*>             units;
		map<Location, CellGroup*> grid;
};
