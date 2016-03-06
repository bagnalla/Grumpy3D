#ifndef GRUMPYCONFIG_H
#define GRUMPYCONFIG_H

namespace grumpy
{
    /** \brief Configuration settings for Grumpy3D.
     */
    class GrumpyConfig
    {
    public:
        static void SetGameSpeed(float s);
        static float GetGameSpeed();

        static float GetGameSpeedFactor();
    private:
        static float gameSpeed;
    };
}

#endif
