#pragma once
#include "App/AppBase.h"

typedef TL::MkTlst<
		DeathZone		
		, DefectSig<Defect>
		, StructSig<Defect>
		, DefectSig<SortDown>		
		, StructSig<SortDown>
		, Nominal
		, SensorOff
	>::Result zone_status_list;