#ifndef RemoteLocker_h
#define RemoteLocker_h

#include "../Team/Team.h"
#include "../DIGI_V6_15/DIGI_V6_15.h"

class RemoteLocker {
 public:
  RemoteLocker( Team* team_a );
  ~RemoteLocker();
  bool playerNotServing( int button );

 private:
  Team* _team_a;
};

#endif
