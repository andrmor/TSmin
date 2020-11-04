#include "SteppingAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

void SteppingAction::UserSteppingAction(const G4Step * step)
{
    if (step->GetTrack()->GetParentID() == 0)
    {
        const G4StepPoint * postP  = step->GetPostStepPoint();
        const G4ThreeVector & pos  = postP->GetPosition();

        SumPath += pos[0];
        ++SumNum;

        const G4ThreeVector & md = postP->GetMomentumDirection();
        std::cout << md[0] << " " <<  md[1] << " " << md[2] << "\n";
    }

    step->GetTrack()->SetTrackStatus(fStopAndKill);
}
