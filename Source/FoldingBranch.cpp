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

    float vk;

    if (absvIn > clip_thresh)
    {
        vk = vIn;
    }
    else
    {
        vk = svIn * clip_thresh;
    }

    if (A >= clip_thresh)
    {
        clipArr[0] = asin((vs * rk_1) / (A * rk_2)) / (2 * pi * f0);
        clipArr[1] = 1 / (2 * f0) - clipArr[0];
        clipArr[2] = 1 / (2 * f0) + clipArr[0];
        clipArr[3] = 1 / f0 - clipArr[0];

        if (ph < delta && flagArr[3])
        {
            for (int i = 0; i < 4; i++)
            {
                flagArr[i] = false;
            }
        }
        else if (ph < delta && flagArr[3])
        {
            float d = 1 - ((1 - clipArr[3] + ph) / delta);

            float h0 = powf(d, 3) * (1 / 6) + 0.5 * (d * d) - 0.5 * d + 1 / 6;
            float h1 = powf(d, 3) * (1 / 6);

            float val = A * std::sin(2 * pi * clipArr[3]);
            float pol = (float(0) < val) - (val < float(0));

            float mu = abs(A * cos(2 * pi * clipArr[3]) * 2 * pi * f0 * T);
            
            if (mu < thresh)
                mu = 0;

            vk = vk + pol * mu * h1;
            vk_n1 = vk_n1 + pol * mu * h0;

            for (int i = 0; i < 4; i++)
            {
                flagArr[i] = false;
            }
        }

        //For every clipping point
        for (int i = 0; i < 4; i++)
        {
            if (ph > clipArr[i] && ph > (ph - delta) && !flagArr[i])
            {
                float d = 1 - (ph - clipArr[i]) / delta;

                float h0 = powf(d, 3) * (1 / 6) + 0.5 * (d * d) - 0.5 * d + 1 / 6;
                float h1 = powf(d, 3) * (1 / 6);

                float val = A * std::sin(2 * pi * clipArr[i]);
                float pol = (float(0) < val) - (val < float(0));

                float mu = abs(A * cos(2 * pi * clipArr[i]) * (2 * pi * f0 * T));

                if (mu < thresh)
                    mu = 0;

                vk = vk + pol * mu * h1;
                vk_n1 = vk_n1 + pol * mu * h0;
                flagArr[i] = true;
            }
        }
        float vk_n1_s = (float(0) < vk_n1) - (vk_n1 < float(0));
        float out = ((rk_2 * rk_3) / (rk_1 * rk_3 + rk_2 * rk_3 + rk_1 * rk_2)) * (vk_n1 - vk_n1_s * clip_thresh);

        vk_n1 = vk;
        ph = fmod(ph + delta, 1.0f);

        return out;
    }

}