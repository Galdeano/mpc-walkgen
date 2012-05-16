#ifndef _SHARED_PG_TYPE_ZEBULON
#define  _SHARED_PG_TYPE_ZEBULON

////////////////////////////////////////////////////////////////////////////////
///
///\file	api.h
///\brief	Definition of zebulon types
///\author	Lafaye Jory
///\author      Keith François
///\version	1.2
///\date	27/04/12
///
////////////////////////////////////////////////////////////////////////////////

#include <mpc-walkgen/common/sharedpgtypes.h>
#include <Eigen/Dense>
#include <vector>





namespace MPCWalkgen{
  namespace Zebulon{
    //
    // Enum types
    //

    /// \name Enum types
    /// \{

    enum BodyType{
      BASE,
      COM
    };

    /// \}

    //
    // Structures
    //

    /// \name Structures
    /// \{



    struct MPC_WALKGEN_API QPPonderation{
      std::vector<double> baseInstantVelocity;
      std::vector<double> CopCentering;
      std::vector<double> CoMCentering;
      std::vector<double> CoMJerkMin;
      std::vector<double> baseJerkMin;
      std::vector<double> OrientationInstantVelocity;
      std::vector<double> OrientationJerkMin;

      /// \brief Define the element of ponderation std::vector used in this iteration
      int activePonderation;

      QPPonderation(int nb = 2);
    };

    struct MPC_WALKGEN_API MPCData{
      // The following parameters are fixed once and for all at initialization
      /// \brief Sampling period considered in the QP
      double QPSamplingPeriod;    //blocked - precomputeObjective
      double MPCSamplingPeriod;   //blocked - precomputeObjective / RigidBodySystem::computeDynamicMatrix
      double actuationSamplingPeriod;   //blocked - precomputeObjective / RigidBodySystem::computeDynamicMatrix

      /// \brief Nb. samplings inside preview window
      int nbSamplesQP;  //blocked - precomputeObjective

      /// \brief Compute the number of recomputations left until next sample
      int nbFeedbackSamplesLeft(double firstSamplingPeriod) const;
      /// \brief number of simulation iterations between two feedback call
      int nbSamplesControl() const;
      /// \brief number of feedback iterations between two QP instants
      int nbFeedbackSamplesStandard() const;

      QPPonderation ponderation;

      MPCData();
    };

    struct MPC_WALKGEN_API RobotData {
      double CoMHeight;
      double robotMass; //TODO: rename mass
      std::vector<double> basePos;
      double b;
      double h;
      std::vector<double> baseLimit;
      std::vector<double> orientationLimit;
      double comLimitX;
      double comLimitY;

      RobotData();
    };

    struct MPC_WALKGEN_API MPCSolution{

      MPCSolution();

      void reset();

      // attributes
      Eigen::VectorXd qpSolution;
      Eigen::VectorXd initialSolution;

      Eigen::VectorXi constraints;
      Eigen::VectorXi initialConstraints;

      Eigen::VectorXd qpSolutionOrientation;
      Eigen::VectorXd initialSolutionOrientation;

      Eigen::VectorXi constraintsOrientation;
      Eigen::VectorXi initialConstraintsOrientation;

      bool useWarmStart;
      /// \brief True if a new trajectory is computed in online loop
      bool newTraj;

      Eigen::VectorXd CoPTrajX;
      Eigen::VectorXd CoPTrajY;

      struct State
      {
        Eigen::VectorXd CoMTrajX_;
        Eigen::VectorXd CoMTrajY_;
        Eigen::VectorXd CoMTrajYaw_;
        Eigen::VectorXd baseTrajX_;
        Eigen::VectorXd baseTrajY_;

      };
      std::vector<State> state_vec;
    };
  }
}

#endif /* _SHARED_PG_TYPE_HUMANOID */