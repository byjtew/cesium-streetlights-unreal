#include "StreetlightStyleConfiguration.h"


void FStreetlightStyleConfiguration::Preset_Default()
{
	Tags = {
		FTagValuesArray{
			TEXT("highway"),
			{
				TEXT("motorway"),
				TEXT("motorway_link"),
				TEXT("trunk"),
				TEXT("trunk_link"),
				TEXT("primary"),
				TEXT("primary_link"),
				TEXT("secondary"),
				TEXT("secondary_link"),
				TEXT("tertiary"),
				TEXT("residential"),
			},
		}
	};
	HeightAboveGround       = 8.0f;
	Intensity               = 120.0f;
	AdditionalSpace         = 5.0;
	SourceRadius            = 20.0f;
	AttenuationRadius       = 40.0f;
	UseTemperatureForColor  = false;
	ColorTemperatureKelvins = 3000.0f;
	Color                   = FColor::White;
	CastShadows             = false;
}

void FStreetlightStyleConfiguration::Preset_Motorway()
{
	Tags = {
		FTagValuesArray{
			TEXT("highway"),
			{
				TEXT("motorway"),
				TEXT("motorway_link"),
				TEXT("trunk"),
				TEXT("trunk_link"),
			},
		}
	};
	HeightAboveGround      = 10.0f;
	Intensity              = 200.0f;
	AdditionalSpace        = 10.0;
	SourceRadius           = 25.0f;
	AttenuationRadius      = 60.0f;
	UseTemperatureForColor = false;
	Color                  = FColor(255, 244, 214); // warm white
	CastShadows            = true;
}

void FStreetlightStyleConfiguration::Preset_Residential()
{
	Tags = {
		FTagValuesArray{
			TEXT("highway"),
			{
				TEXT("residential"),
				TEXT("tertiary"),
				TEXT("secondary"),
			},
		}
	};
	HeightAboveGround       = 6.0f;
	Intensity               = 100.0f;
	AdditionalSpace         = 4.0;
	SourceRadius            = 15.0f;
	AttenuationRadius       = 30.0f;
	UseTemperatureForColor  = true;
	ColorTemperatureKelvins = 3000.0f;
	CastShadows             = false;
}

void FStreetlightStyleConfiguration::Preset_Alley()
{
	Tags = {
		FTagValuesArray{
			TEXT("highway"),
			{
				TEXT("residential"),
				TEXT("living_street"),
				TEXT("service"),
			},
		}
	};
	HeightAboveGround       = 4.0f;
	Intensity               = 60.0f;
	AdditionalSpace         = 2.0;
	SourceRadius            = 8.0f;
	AttenuationRadius       = 15.0f;
	UseTemperatureForColor  = true;
	ColorTemperatureKelvins = 2700.0f;
	CastShadows             = false;
}

void FStreetlightStyleConfiguration::Preset_Railway()
{
	Tags = {
		FTagValuesArray{
			TEXT("railway"),
			{
				TEXT("rail"),
			},
		}
	};
	HeightAboveGround      = 12.0f;
	Intensity              = 150.0f;
	AdditionalSpace        = 6.0;
	SourceRadius           = 30.0f;
	AttenuationRadius      = 80.0f;
	UseTemperatureForColor = false;
	Color                  = FColor::White;
	CastShadows            = true;
}


FColor FStreetlightStyleConfiguration::GetLightColor() const
{
	if (not UseTemperatureForColor)
	{
		return Color;
	}

	float kelvins = FMath::Clamp(ColorTemperatureKelvins, 1000.0f, 40000.0f) / 100.0f;
	float r, g, b;
	{
		if (kelvins <= 66.0f)
			r = 1.0f;
		else
			r = FMath::Clamp(1.29293618606f * FMath::Pow(kelvins - 60.0f, -0.1332047592f), 0.0f, 1.0f);
	}
	{
		if (kelvins <= 66.0f)
			g = FMath::Clamp(0.390081578769f * FMath::Loge(kelvins) - 0.631841443788f, 0.0f, 1.0f);
		else
			g = FMath::Clamp(1.12989086089f * FMath::Pow(kelvins - 60.0f, -0.0755148492f), 0.0f, 1.0f);
	}
	{
		if (kelvins >= 66.0f)
			b = 1.0f;
		else if (kelvins <= 19.0f)
			b = 0.0f;
		else
			b = FMath::Clamp(0.54320678911f * FMath::Loge(kelvins - 10.0f) - 1.19625408914f, 0.0f, 1.0f);
	}

	return FLinearColor(r, g, b).ToFColor(false);
}
