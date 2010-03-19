#ifndef StatusLineUpdater_h__
#define StatusLineUpdater_h__

#include "Tetrahedralizer.h"

class StatusLineUpdater
    : public Tetrahedralizer::ProgressListenerInterface
{
public:
    virtual void begin();
    virtual void end();
    virtual bool update(int percentDone);
};

#endif // StatusLineUpdater_h__