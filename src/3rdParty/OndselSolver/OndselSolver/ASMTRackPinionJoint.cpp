/***************************************************************************
 *   Copyright (c) 2023 Ondsel, Inc.                                       *
 *                                                                         *
 *   This file is part of OndselSolver.                                    *
 *                                                                         *
 *   See LICENSE file for details about copyright.                         *
 ***************************************************************************/
#include <fstream>	

#include "ASMTRackPinionJoint.h"
#include "RackPinJoint.h"

using namespace MbD;

std::shared_ptr<Joint> MbD::ASMTRackPinionJoint::mbdClassNew()
{
    return CREATE<RackPinJoint>::With();
}

void MbD::ASMTRackPinionJoint::parseASMT(std::vector<std::string>& lines)
{
	ASMTJoint::parseASMT(lines);
	readPitchRadius(lines);
}

void MbD::ASMTRackPinionJoint::readPitchRadius(std::vector<std::string>& lines)
{
	if (lines[0].find("pitchRadius") == std::string::npos) {
		pitchRadius = 0.0;
	}
	else {
		lines.erase(lines.begin());
		pitchRadius = readDouble(lines[0]);
		lines.erase(lines.begin());
	}
}

void MbD::ASMTRackPinionJoint::createMbD(std::shared_ptr<System> mbdSys, std::shared_ptr<Units> mbdUnits)
{
	ASMTJoint::createMbD(mbdSys, mbdUnits);
	auto rackPinJoint = std::static_pointer_cast<RackPinJoint>(mbdObject);
	rackPinJoint->pitchRadius = pitchRadius;
}

void MbD::ASMTRackPinionJoint::storeOnLevel(std::ofstream& os, int level)
{
	ASMTJoint::storeOnLevel(os, level);
	storeOnLevelString(os, level + 1, "pitchRadius");
	storeOnLevelDouble(os, level + 2, pitchRadius);
	//storeOnLevelString(os, level + 1, "constant");
	//storeOnLevelDouble(os, level + 2, aConstant);
}
