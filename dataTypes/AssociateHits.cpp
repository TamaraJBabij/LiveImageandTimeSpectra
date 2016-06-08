
#include "DataSet.h"
#include <vector>
#include "Hit.h"
#include "Group.h"


//Takes hits within a group and associates them with a set
// set is MCP hit with associated layer hits.
// format group -> set -> hit (channel, detector id, time)