#ifndef RIGID_BODY
#define RIGID_BODY

////////////////////////////////////////////////////////////////////////////////
///
///\file	rigid-body.h
///\brief	A class to store rigid bodies
///\author	Lafaye Jory
///\version	1.0
///\date	02/05/12
///
////////////////////////////////////////////////////////////////////////////////


#include "types.h"
#include "../common/interpolation.h"


namespace MPCWalkgen{
  namespace Zebulon{
    class RigidBody{

    public:
      RigidBody(const MPCData * generalData,
                const RobotData * robotData,
                const Interpolation * interpolation);
      virtual ~RigidBody();

      inline const BodyState & state() const{return state_;}
      inline BodyState & state(){return state_;}

      inline void state(const BodyState & s){state_=s;}

      const LinearDynamics & dynamics(DynamicMatrixType type) const;

      void computeDynamics();

      virtual void interpolate(MPCSolution & result, double currentTime, const VelReference & velRef)=0;

    protected:
      virtual void computeDynamicsMatrices(LinearDynamics & dyn,
                                           double S, double T, int N, DynamicMatrixType type)=0;


    protected:
      const MPCData *generalData_;
      const RobotData *robotData_;
      const Interpolation *interpolation_;

      BodyState state_;
      LinearDynamics pos_vec_;
      LinearDynamics vel_vec_;
      LinearDynamics acc_vec_;
      LinearDynamics jerk_vec_;
      LinearDynamics cop_vec_;

      LinearDynamics posInterpol_;
      LinearDynamics velInterpol_;
      LinearDynamics accInterpol_;
      LinearDynamics copInterpol_;

      int matrixNumber_;
    };
  }
}

#endif //RIGID_BODY