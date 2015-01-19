#pragma once

/**
 * \brief Supported sources of systematical variations
 * 
 * They all are statistically independent. The sources whose names start with BTag are related to
 * b-tagging uncertainties. However, the JEC uncertainty affects b-tagging as well.
 */
enum class SystType
{
    Nominal,
    JEC,
    BTagPurityHF,
    BTagPurityLF,
    BTagStatHF1,
    BTagStatHF2,
    BTagStatLF1,
    BTagStatLF2,
    BTagCharmUnc1,
    BTagCharmUnc2
};


/// Directions of systematical variations
enum class SystDirection
{
    Up,
    Down
};
