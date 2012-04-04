#include "structures.h"
#include "CellGroup.h"
#include "Level.h"
#include <vector>

class Level;
class CellGroup; //Forward declarations

class Event {
  
 public:
  static enum Type {PULSE, DIE, OPEN};

  Event( std::vector<Location> locs, enum Type type, CellGroup* sender );
  
  void send( Level* receiver );

 private:
  std::vector<Location> locations;
  enum Type type;
  CellGroup* sender;

};
