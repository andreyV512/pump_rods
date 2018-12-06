#pragma once
#include "App/AppBase.h"

typedef TL::MkTlst<
		Nominal
		, DefectSig<SortDown>
		, DefectSig<Defect>
		, StructSig<SortDown>
		, StructSig<Defect>
		, DeathZone
	>::Result zone_status_list;