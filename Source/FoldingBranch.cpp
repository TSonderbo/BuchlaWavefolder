/*
  ==============================================================================

	FoldingBranch.cpp
	Created: 5 Feb 2023 11:31:52am
	Author:  Sonderbo

  ==============================================================================
*/


#include "FoldingBranch.h"


FoldingBranch::FoldingBranch(float rk_1, float rk_3)
{
	this->rk_1 = rk_1;
	this->rk_3 = rk_3;
	this->clip_thresh = vs * (rk_1 / rk_2);
}

void FoldingBranch::setFundemental(float f0)
{
	this->f0 = f0;
	delta = f0 / sampleRate;
}

void FoldingBranch::prepareToPlay(double sampleRate)
{
	this->sampleRate = sampleRate;
}

void FoldingBranch::setAmplitude(float A)
{
	this->A = A;
}

float FoldingBranch::processSample(float vIn)
{
	float absvIn = abs(vIn);
	float svIn = (float(0) < vIn) - (vIn < float(0));

	float vk = (absvIn > clip_thresh) ? vIn : svIn * clip_thresh;

	if (A >= clip_thresh)
	{
		//Calculate clipping points
		clipArr[0] = asin((vs * rk_1) / (A * rk_2)) / (2.0f * pi * f0);
		clipArr[1] = 1.0f / (2.0f * f0) - clipArr[0];
		clipArr[2] = 1.0f / (2.0f * f0) + clipArr[0];
		clipArr[3] = 1.0f / f0 - clipArr[0];

		if (period_ts < delta && flagArr[3])
		{
			//reset clipping point flags
			for (int i = 0; i < 4; i++)
			{
				flagArr[i] = false;
			}
		}
		else if (period_ts < delta && !flagArr[3]) //at start of signal period
		{
			float d = 1 - ((1 - clipArr[3] + period_ts) / delta); //fractional delay

			float mu = abs(A * cos(2.0f * pi * clipArr[3]) * 2.0f * pi * f0 * T); //polyBLAMP scaling factor

			float p0 = mu * (powf(-d, 3.0f) * (1.0f / 6.0f) + 0.5f * (d * d) - 0.5f * d + 1.0f / 6.0f); //polyBLAMP coefficient
			float p1 = mu * (powf(d, 3.0f) * (1.0f / 6.0f)); //polyBLAMP coefficient

			float val = A * std::sin(2.0f * pi * clipArr[3]); //Get clipping point value
			float pol = (float(0) < val) - (val < float(0)); //Get polarity of clipping point

			vk = vk + pol * p1; 
			vk_n1 = vk_n1 + pol * p0; 

			//reset clipping point flags
			for (int i = 0; i < 4; i++)
			{
				flagArr[i] = false;
			}
		}

		//For every clipping point
		for (int i = 0; i < 4; i++)
		{
			if (period_ts > clipArr[i] && !flagArr[i])
			{
				float d = 1 - ((period_ts - clipArr[i]) / delta); //fractional delay

				float mu = abs(A * cos(2.0f * pi * clipArr[i]) * (2.0f * pi * f0 * T)); //polyBLAMP scaling factor

				float p0 = mu * (powf(-d, 3.0f) * (1.0f / 6.0f) + 0.5f * (d * d) - 0.5f * d + 1.0f / 6.0f); //polyBLAMP coefficient
				float p1 = mu * (powf(d, 3.0f) * (1.0f / 6.0f)); //polyBLAMP coefficient

				float val = A * std::sin(2 * pi * clipArr[i]); //Get clipping point value
				float pol = (float(0) < val) - (val < float(0)); //Get polarity of clipping point

				vk = vk + pol * p1; 
				vk_n1 = vk_n1 + pol * p0; 
				flagArr[i] = true; //Set clipping point to processed
			}
		}
	}

	float vk_n1_s = (float(0) < vk_n1) - (vk_n1 < float(0));
	float out = ((rk_2 * rk_3) / (rk_1 * rk_3 + rk_2 * rk_3 + rk_1 * rk_2)) * (vk_n1 - vk_n1_s * clip_thresh);

	vk_n1 = vk;
	period_ts = fmod(period_ts + delta, 1.0f);

	return out;
}