#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"

class G4Step;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(double & sumPath, int & sumNum) : SumPath(sumPath), SumNum(sumNum) {}

    virtual void UserSteppingAction(const G4Step * step) override;

private:
    double & SumPath;
    int    & SumNum;
};

#endif // SteppingAction_h
